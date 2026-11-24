# Checks of sc_out.txt (tb_sc.v): vsync position in its line (field parity),
# lengths of the lines around each vsync, of the picture lines, resyncs
import sys, bisect
H, V, D, F, R = [], [], [], [], []
for l in open('sc_out.txt'):
    k, t = l.split(); t = int(t)
    {'H': H, 'V': V, 'D': D, 'F': F, 'R': R}[k].append(t)
htot = int(sys.argv[1]) if len(sys.argv) > 1 else 1717
print('resyncs at', R, ' input frames', len(F))
for v in V[1:]:
    i = bisect.bisect_right(H, v) - 1           # last hsync at or before the vsync edge
    if i < 12 or i + 12 >= len(H): continue
    ofs = v - H[i]
    lens = [H[j + 1] - H[j] for j in range(i - 8, i + 12)]
    print('V at %8d: %4d clocks after hsync (%s)  lines -8..+12: %s' % (v, ofs, 'line start' if ofs < htot // 4 else 'half line', ' '.join(str(x) for x in lens)))
dl = sorted(set(D[j + 1] - D[j] for j in range(len(D) - 1) if D[j + 1] - D[j] < 2 * htot))
print('picture line lengths (DE to DE):', dl)
allh = [H[j + 1] - H[j] for j in range(len(H) - 1)]
print('line lengths (all, hsync to hsync): min %d max %d, distinct %s' % (min(allh), max(allh), sorted(set(allh))[:12]))
# output frame start (first hsync after a vsync? use vsync of odd field) vs input frame start
iv = [V[j + 1] - V[j] for j in range(len(V) - 1)][2:]
print('vsync intervals (fields):', iv[-6:], ' input frame:', F[-1] - F[-2] if len(F) > 1 else '-')
