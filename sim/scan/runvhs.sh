#!/bin/bash
# VHS firmware: exact CEA output line lengths (1716 / 1728, Line2x 858 / 864),
# input with the VHS model of tb_lb.v (line jitter, drift, head switch,
# frame start jitter), 525 and 625 lines, Line1x (Archive) and Line2x (View)
run() {
  n=$1; a=$2; r=$3; h=$4
  ./obj_dir/tblb $a +HTOT=$h > /dev/null
  lb=$(python3 anlb.py $r | tail -1)
  vi=$(python3 - $h $r <<'P'
import sys, bisect
H,V,D,R=[],[],[],[]
for l in open('sc_out.txt'):
    k,t=l.split(); t=int(t)
    {'H':H,'V':V,'D':D,'R':R,'F':[]}[k].append(t)
h=int(sys.argv[1])
# output lines: picture lines (DE to DE) and all lines (hsync to hsync)
dl=sorted(set(D[j+1]-D[j] for j in range(len(D)-1) if D[j+1]-D[j] < 2*h))
al=[H[j+1]-H[j] for j in range(20,len(H)-1)]
par=[]
for v in V[3:]:
    i=bisect.bisect_right(H,v)-1
    par.append(1 if (v-H[i]) > h//4 else 0)
alt=all(par[j]!=par[j+1] for j in range(len(par)-1))
same=all(p==0 for p in par)
iv=sorted(set(V[j+1]-V[j] for j in range(3,len(V)-1)))
print('resyncs %d | picture lines %s | all lines %d..%d | vsync %s | fields %d..%d' % (len(R), dl, min(al), max(al),
      'alternating' if alt else ('line start' if same else 'PARITY ERROR'), iv[0], iv[-1]))
P
)
  echo "$n: $lb | $vi"
}
NF=${NF:-40}; LS=${LS:-3}
P576="+LINES=625 +VTOT=625 +VACT=288 +VBP=19 +IVBP=19 +IVACT=288 +HSL=126 +HBP=138 +IHSL=126 +IHBP=138"
L2N="+VTOT=525 +VACT=480 +VSL=6 +VBP=30 +ILACE=0 +YRPT=1 +VSTART=523 +HACT=720 +HSL=62 +HBP=60 +IHSKIP=1 +IHSL=62 +IHBP=57 +IHACT=720"
L2P="+LINES=625 +VTOT=625 +VACT=576 +VSL=5 +VBP=39 +ILACE=0 +YRPT=1 +VSTART=623 +HACT=720 +HSL=64 +HBP=68 +IHSKIP=1 +IHSL=63 +IHBP=69 +IHACT=720 +IVBP=19 +IVACT=288"
V="+VJ=${VJ:-8} +VA=${VA:-150} +VP=${VP:-200} +VHS=${VHS:-150} +VHSL=6 +JIT=${JIT:-50} +LS=$LS"
for lp in 0 100 -100 300; do
  LPN=$((439296 + 439296*lp/1000000)); LPP=$((442368 + 442368*lp/1000000))
  run "480i Archive ${lp}ppm" "+NF=$NF +LP=$LPN $V" 0 1716
  run "576i Archive ${lp}ppm" "+NF=$NF +LP=$LPP $P576 $V" 0 1728
  run "480i View    ${lp}ppm" "+NF=$NF +LP=$LPN $L2N $V" 1 858
  run "576i View    ${lp}ppm" "+NF=$NF +LP=$LPP $L2P $V" 1 864
done
