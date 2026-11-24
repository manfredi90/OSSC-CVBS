# Checks of lb_out.txt (tb_lb.v): every output picture line shows one input
# line (same tag at its first / middle / last pixel), in order (Y_RPT+1 output
# lines per input line, from the first one of the field), data age (input clocks)
import sys
rpt = int(sys.argv[1]) if len(sys.argv) > 1 else 0
skip = int(sys.argv[2]) if len(sys.argv) > 2 else 4      # output fields skipped (lock)
rows = [list(map(int, l.split())) for l in open('lb_out.txt')]
lines = {}; order = []
for t, fid, vc, x, tf, tl, age in rows:
    key = (len(order), vc)
    if not order or order[-1][1] != vc: order.append((t, vc, fid, []))
    order[-1][3].append((x, tf, tl, age))
fields = []; cur = []
for o in order:
    if cur and o[1] < cur[-1][1]: fields.append(cur); cur = []
    cur.append(o)
fields.append(cur)
bad = 0; ages = []; nf = 0
for fi, fl in enumerate(fields[skip:-1]):
    nf += 1
    seq = []
    for j, (t, vc, fid, px) in enumerate(fl):
        tags = set((p[1], p[2]) for p in px)
        ages += [p[3] for p in px]
        if len(tags) != 1:
            bad += 1
            if bad <= 8: print('field %d line %d: torn %s' % (fi, j, sorted(tags)))
        seq.append(sorted(tags)[0])
    # bob (Line2x+ of interlaced sources): the first line may show the last
    # line of the previous field
    if len(seq) > 1 and seq[0][0] != seq[1][0]: seq = seq[1:]
    runs = []
    for j, s_ in enumerate(seq):
        if j and (s_[0] != seq[0][0] or s_[1] - seq[j-1][1] not in (0, 1)):
            bad += 1
            if bad <= 8: print('field %d line %d: %s after %s' % (fi, j, s_, seq[j-1]))
        if j and s_ == seq[j-1]: runs[-1] += 1
        else: runs.append(1)
    if any(r != rpt + 1 for r in runs[1:-1]):
        bad += 1; print('field %d: repeat counts %s' % (fi, sorted(set(runs))))
    if seq[0][1] != 0 and not (len(runs) and runs[0] < rpt + 1 and seq[0][1] == 0): print('field %d starts at input line %d' % (fi, seq[0][1]))
print('fields checked %d, bad lines %d, data age min %d max %d (input clocks)' % (nf, bad, min(ages), max(ages)))
