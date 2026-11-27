#!/bin/sh
# field parity in the frontend: per case alternation / placement errors and
# fields read as progressive (should all be 0; a dropped field: 1 placement
# for the field after the splice at most)
cd "$(dirname "$0")"
verilator --binary --timing -Wno-fatal -Wno-lint -Wno-style -Wno-PROCASSWIRE --top-module tb -I../../rtl tb_fid.v ../../rtl/tvp7002_frontend.v -o tbfid >/dev/null || exit 1
for P in 1 0; do
  for a in "" "+VSD=0 +VSJ=4" "+VSD=-6 +VSJ=6" "+VSD=300 +VSJ=10" "+VSD=0 +VSJ=6 +VHS=150 +VA=150" "+VSD=0 +VSJ=4 +SKIP=20" "+SKIP=21"; do
    printf "PAL=%s %-32s " "$P" "$a"; ./obj_dir/tbfid +PAL=$P +NF=50 $a | grep SUMMARY
  done
done
