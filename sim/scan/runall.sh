#!/bin/bash
# name | tb args | rpt | htot_out
run() {
  n=$1; a=$2; r=$3; h=$4
  ./obj_dir/tblb $a +HTOT=$h > /dev/null
  lb=$(python3 anlb.py $r | tail -1)
  vi=$(python3 - $h <<'P'
import sys
H,V,R=[],[],[]
for l in open('sc_out.txt'):
    k,t=l.split(); t=int(t)
    if k=='H':H.append(t)
    if k=='V':V.append(t)
    if k=='R':R.append(t)
iv=[V[j+1]-V[j] for j in range(len(V)-1)][4:]
allh=sorted(set(H[j+1]-H[j] for j in range(20,len(H)-1)))
import bisect
h=int(sys.argv[1]); par=[]
for v in V[3:]:
    i=bisect.bisect_right(H,v)-1
    par.append(1 if (v-H[i]) > h//4 else 0)
alt=all(par[j]!=par[j+1] for j in range(len(par)-1))
same=all(p==par[0] for p in par)
print('resyncs %d, field lengths %s, line lengths %s, vsync %s' % (len(R), sorted(set(iv)), allh, 'alternating (interlaced ok)' if alt else ('always line start' if same and par[0]==0 else 'PARITY ERROR')))
P
)
  echo "$n: $lb | $vi"
}
NF=${NF:-40}
run "480i Wii 1716.02"  "+NF=$NF +LP=439301" 0 1716
run "480i 1716.5 (worst)" "+NF=$NF +LP=439424" 0 1716
run "480i 1716.5 htot1717" "+NF=$NF +LP=439424" 0 1717
run "240p SNES 1714.7" "+NF=$NF +LP=438963 +IPROG=1 +ILACE=0 +VTOT=262" 0 1715
run "240p PCE 1716.2" "+NF=$NF +LP=439347 +IPROG=1 +ILACE=0 +VTOT=262" 0 1716
run "240p MD 1719.8" "+NF=$NF +LP=440269 +IPROG=1 +ILACE=0 +VTOT=262" 0 1720
run "576i PAL 1728.5 (worst)" "+NF=$NF +LINES=625 +LP=442496 +VTOT=625 +VACT=288 +VBP=19 +IVBP=19 +IVACT=288 +HSL=126 +HBP=138 +IHSL=126 +IHBP=138" 0 1728
run "288p PAL 1728.3" "+NF=$NF +LP=442445 +IPROG=1 +ILACE=0 +VTOT=312 +LINES=312 +VACT=288 +VBP=19 +IVBP=19 +IVACT=288 +HSL=126 +HBP=138 +IHSL=126 +IHBP=138" 0 1728
L2="+VTOT=525 +VACT=480 +VSL=6 +VBP=30 +ILACE=0 +YRPT=1 +VSTART=523 +HACT=720 +HSL=62 +HBP=60 +IHSKIP=1 +IHSL=62 +IHBP=57 +IHACT=720"
run "Line2x 480i Wii" "+NF=$NF +LP=439301 $L2" 1 858
run "Line2x 240p SNES" "+NF=$NF +LP=438963 +IPROG=1 $L2" 1 857
run "Line2x 480i 1717.0 (worst)" "+NF=$NF +LP=439552 $L2" 1 858
L3="+OMX2=3 +VTOT=786 +VACT=720 +VSL=9 +VBP=45 +ILACE=0 +YRPT=2 +VSTART=783 +HACT=720 +HSL=62 +HBP=60 +IHSKIP=1 +IHSL=62 +IHBP=57 +IHACT=720"
run "Line3x 240p SNES" "+NF=$NF +LP=438963 +IPROG=1 $L3" 2 857
L4="+OMX2=4 +VTOT=1048 +VACT=960 +VSL=12 +VBP=60 +ILACE=0 +YRPT=3 +VSTART=1044 +HACT=720 +HSL=62 +HBP=60 +IHSKIP=1 +IHSL=62 +IHBP=57 +IHACT=720"
run "Line4x 240p SNES" "+NF=$NF +LP=438963 +IPROG=1 $L4" 3 857
run "Line4x 240p 1717.0 (worst)" "+NF=$NF +LP=439552 +IPROG=1 $L4" 3 858
