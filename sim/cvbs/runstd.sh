#!/bin/bash
# VHS firmware: every colour standard, clean and from a tape (timebase drift
# +-3us over 100 lines, 20ns line jitter, chroma phase error 8 degrees,
# subcarrier +30ppm, noise), firmware defaults (crystal clock, reference
# filter on, PAL delay line on)
TAPE="--vhs-ns 3000 --vhs-period 100 --vhs-jit 20 --cphase 8 --fsc-ppm 30 --noise 1.5"
for std in ntsc pal pal60 ntsc443 palm paln; do
  for cond in clean tape; do
    a="--std $std --xtal --reffilt 1 --dl 1"; [ $cond = tape ] && a="$a $TAPE"
    r=$(timeout 900 python3 cvbs_sim.py $a 2>&1 | grep -E 'WORST|BAR ERR|LINE JITTER:' | sed 's/(phase slope.*//' | tr '\n' ' ')
    printf "%-8s %-5s %s\n" $std $cond "$r"
  done
done
