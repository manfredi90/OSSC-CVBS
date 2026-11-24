#!/usr/bin/env python3
"""Composite / S-Video test signal generator and checker for cvbs_decoder.v

Generates line-locked TVP7002-like samples (8-bit), runs the RTL with
iverilog and measures the decoded colour bars.
"""
import argparse, math, os, subprocess, sys
import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))

FSC = {'ntsc': 3579545.45, 'pal': 4433618.75, 'pal60': 4433618.75,
       'ntsc443': 4433618.75, 'palm': 3575611.49, 'paln': 3582056.25}

# 75% colour bars (R, G, B 0..1): white, yellow, cyan, green, magenta, red, blue, black
BARS = [(.75, .75, .75), (.75, .75, 0), (0, .75, .75), (0, .75, 0),
        (.75, 0, .75), (.75, 0, 0), (0, 0, .75), (0, 0, 0)]


def cfg_for(std, svideo=False, bstart=163, bright=0, nonotch=False, loop_off=False, edge=0):
    pal = std in ('pal', 'pal60', 'palm', 'paln')
    bp443 = FSC[std] > 4e6
    cfg = (1 | (svideo << 1) | (pal << 2) | (bp443 << 3) | (nonotch << 4) |
           (loop_off << 5) | (bstart << 8) | ((bright & 0x3f) << 16) | (edge << 22))
    return cfg, pal


def hc_for(pal, sat=1.0, hue_deg=0.0, lg=51):
    k = 77.3 if pal else 102.0
    re = int(round(k * sat * math.cos(math.radians(-hue_deg))))
    im = int(round(k * sat * math.sin(math.radians(-hue_deg))))
    return (re & 0x3ff) | ((im & 0x3ff) << 16) | ((lg - 32) << 26)


def gen(args):
    std = args.std
    lines625 = std in ('pal', 'paln')
    N = 1728 if lines625 else 1716
    if args.fh:
        fh = args.fh
    else:
        fh = 15625.0 if lines625 else (FSC['ntsc'] / 227.5 if std == 'ntsc' else 15734.26)
    fsc = FSC[std] * (1 + args.fsc_ppm * 1e-6)
    fs = N * fh
    if args.xtal:
        fs = 27e6                       # crystal sample clock, not line-locked
    pal = std in ('pal', 'pal60', 'palm', 'paln')
    os_ = args.os                           # oversampling for the analog model
    t_line = 1.0 / fh
    us = 1e-6
    cpv = args.codes_per_v
    blank_code = args.blank_code

    sync_w = 4.7 * us
    b_start = (5.6 if pal else 5.3) * us
    b_cyc = 10 if pal else 9
    act0 = (10.5 if pal else 9.4) * us
    act1 = t_line - 1.5 * us
    a_burst = 0.15 if pal else 0.14
    a_burst *= args.burst_scale

    tt = (np.arange(N * os_) / (fs * os_))  # time inside a line
    cv_all, c_all = [], []
    hs_all = []
    rng = np.random.default_rng(1)
    for ln in range(args.lines):
        t0 = ln * t_line
        if args.xtal:
            # samples k = ceil(ln*T*fs) .. ceil((ln+1)*T*fs)-1 of a continuous
            # 27MHz clock: N or N+1 samples per line, the line starts at a
            # different fraction of a sample on each line
            k0 = math.ceil(ln * t_line * fs)
            k1 = math.ceil((ln + 1) * t_line * fs)
            tt = (k0 * os_ + np.arange((k1 - k0) * os_)) / (fs * os_) - t0
        # line-locked clock jitter: the sampling instants of a line are
        # shifted by a random amount (H-PLL phase noise)
        jit = rng.normal(0, args.jitter_ns * 1e-9) if args.jitter_ns else 0.0
        # H-PLL: the sampling clock is continuous; each line lasts N clocks,
        # whose period changes at every hsync (VCO frequency error eps, from
        # the phase detector correction plus noise). The line starts where the
        # previous one ended (timing error e), the loop pulls e back slowly.
        if args.vco_ppm:
            if ln == 0:
                pll_e = 0.0
            else:
                pll_e += t_line * pll_eps
            pll_eps = -args.pll_gain * pll_e / t_line + rng.normal(0, args.vco_ppm * 1e-6)
            eps = pll_eps
            t0 = ln * t_line + pll_e
        else:
            eps = 0.0
        ta = tt * (1 + eps)
        t = t0 + ta + jit
        ph = 2 * math.pi * fsc * t + args.phase0
        vsw = 1 if (not pal or (ln % 2 == 0)) else -1   # PAL: +V on even lines
        y = np.zeros_like(tt)
        c = np.zeros_like(tt)
        y[ta < sync_w] = -0.3
        fl = 312 if lines625 else 262
        vbl = args.vblank and (ln % fl) < 9
        bm = (ta >= b_start) & (ta < b_start + b_cyc / fsc) & (not vbl) & (not args.bw)
        if pal:
            # amplitude a_burst, phase 135 / 225 degrees
            c[bm] = a_burst * (-np.sin(ph[bm]) + vsw * np.cos(ph[bm])) / math.sqrt(2)
        else:
            c[bm] = -a_burst * np.sin(ph[bm])
        am = (ta >= act0) & (ta < act1) & (not vbl)
        pos = (ta[am] - act0) / (act1 - act0)
        bar = np.minimum((pos * 8).astype(int), 7)
        rgb = np.array(BARS)[bar]
        if args.stripes:
            # black / white stripes, 1.5us wide (sharp luma edges)
            k_ = ((ta[am] - act0) / 1.5e-6).astype(int) % 2
            rgb = np.stack([k_ * 0.75] * 3, axis=1)
        Y = 0.299 * rgb[:, 0] + 0.587 * rgb[:, 1] + 0.114 * rgb[:, 2]
        U = 0.492 * (rgb[:, 2] - Y)
        V = 0.877 * (rgb[:, 0] - Y)
        y[am] = 0.7 * (args.setup + (1 - args.setup) * Y)
        c[am] = (0 if args.bw else 0.7) * args.chroma_scale * (U * np.sin(ph[am]) + vsw * V * np.cos(ph[am]))
        if args.svideo:
            cv = y
            cc = c
        else:
            cv = y + c
            cc = np.zeros_like(c)
        cv_all.append(cv)
        c_all.append(cc)
        hs_all.append((ta[::os_] >= sync_w).astype(int))
    cv = np.concatenate(cv_all)
    cc = np.concatenate(c_all)
    # analog bandwidth (~6MHz, gaussian) then sampling
    k = np.arange(-3 * os_, 3 * os_ + 1)
    sig = fs * os_ / (2 * math.pi * 6e6)
    g = np.exp(-0.5 * (k / sig) ** 2)
    g /= g.sum()
    cv = np.convolve(cv, g, mode='same')[::os_]
    cc = np.convolve(cc, g, mode='same')[::os_]
    G = np.clip(np.round(blank_code + cv * cpv + rng.normal(0, args.noise, cv.shape)), 0, 255).astype(int)
    B = np.clip(np.round(128 + cc * cpv + rng.normal(0, args.noise, cc.shape)), 0, 255).astype(int)
    hs = np.concatenate(hs_all)
    lno = np.concatenate([np.full(len(h_), i) for i, h_ in enumerate(hs_all)])
    fl = 312 if lines625 else 262
    vs = np.ones_like(hs)
    if args.vblank:
        vs[(lno % fl) < 3] = 0
    inc = int(round(FSC[std] * (1 + args.osc_ppm * 1e-6) / fs * 2 ** 32)) & 0xffffffff
    return G, B, hs, vs, N, inc, pal


def run(args):
    G, B, hs, vs, N, inc, pal = gen(args)
    cfg, pal = cfg_for(args.std, args.svideo, args.bstart, nonotch=args.nonotch, loop_off=args.loop_off, edge=args.edge)
    hc = hc_for(pal, args.sat, args.hue) | ((args.reffilt & 1) << 10) | ((args.sharp & 7) << 11)
    cfg |= (1 << 7) if args.diag else 0
    cfg |= (1 << 6) if (args.xtal and not args.noalign) else 0
    with open(os.path.join(ROOT, 'in.txt'), 'w') as f:
        f.write('%06x %08x %08x\n' % (cfg, hc, inc))
        for a, b, c, d in zip(G, B, hs, vs):
            f.write('%02x %02x %x %x\n' % (a, b, c, d))
    exe = os.path.join(HERE, 'tb.vvp')
    subprocess.run(['iverilog', '-g2005'] + os.environ.get('IVFLAGS', '').split() + ['-o', exe, os.path.join(HERE, 'tb_cvbs.v'),
                    os.environ.get('CVBS_RTL', os.path.join(ROOT, 'rtl', 'cvbs_decoder.v'))], check=True)
    subprocess.run(['vvp', '-n', exe], check=True, cwd=ROOT, stdout=subprocess.DEVNULL)
    rows = [l.replace('x', '0').split() for l in open(os.path.join(ROOT, 'out.txt'))]
    os.replace(os.path.join(ROOT, 'out.txt'), os.path.join(HERE, 'out.txt'))
    os.replace(os.path.join(ROOT, 'in.txt'), os.path.join(HERE, 'in.txt'))
    R = np.array([int(r[0], 16) for r in rows])
    Gq = np.array([int(r[1], 16) for r in rows])
    Bq = np.array([int(r[2], 16) for r in rows])
    side = np.array([int(r[3], 16) for r in rows])
    st = np.array([int(r[4], 16) for r in rows])
    hs_o = (side >> 1) & 1
    starts = np.where((hs_o[:-1] == 1) & (hs_o[1:] == 0))[0] + 1
    return R, Gq, Bq, starts, st, N, pal


def measure(R, G, B, starts, N, pal, line):
    s = starts[line]
    fs_us = N / 64.0 if pal else N / 63.556
    act0 = (10.5 if pal else 9.4)
    act1 = (64.0 if pal else 63.556) - 1.5
    res = []
    for i in range(8):
        c = act0 + (i + 0.5) * (act1 - act0) / 8
        a = s + int((c - 1.2) * fs_us)
        b = s + int((c + 1.2) * fs_us)
        res.append((R[a:b].mean(), G[a:b].mean(), B[a:b].mean(), max(R[a:b].std(), G[a:b].std(), B[a:b].std())))
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--std', default='pal', choices=list(FSC))
    ap.add_argument('--lines', type=int, default=80)
    ap.add_argument('--svideo', action='store_true')
    ap.add_argument('--fsc-ppm', type=float, default=0.0, help='source subcarrier error')
    ap.add_argument('--osc-ppm', type=float, default=0.0, help='error of the increment (OSSC crystal)')
    ap.add_argument('--fh', type=float, default=0.0, help='line rate (default: standard)')
    ap.add_argument('--noise', type=float, default=0.5)
    ap.add_argument('--codes-per-v', type=float, default=228.6)
    ap.add_argument('--blank-code', type=float, default=64)
    ap.add_argument('--burst-scale', type=float, default=1.0)
    ap.add_argument('--chroma-scale', type=float, default=1.0)
    ap.add_argument('--phase0', type=float, default=0.3)
    ap.add_argument("--bstart", type=int, default=163)
    ap.add_argument('--sat', type=float, default=1.0)
    ap.add_argument('--hue', type=float, default=0.0)
    ap.add_argument('--nonotch', action='store_true')
    ap.add_argument('--loop-off', action='store_true')
    ap.add_argument('--vblank', action='store_true', help='fields with 9 blanked lines (no burst) and vsync')
    ap.add_argument('--bw', action='store_true', help='no colour (no burst)')
    ap.add_argument('--noalign', action='store_true')
    ap.add_argument('--os', type=int, default=4, help='oversampling of the analog model (edges quantised to 1/os sample)')
    ap.add_argument('--xtal', action='store_true', help='27MHz crystal sampling (not line-locked)')
    ap.add_argument('--pll-gain', type=float, default=0.2, help='H-PLL phase correction per line')
    ap.add_argument('--vco-ppm', type=float, default=0.0, help='per-line sampling clock frequency error (ppm rms)')
    ap.add_argument('--jitter-ns', type=float, default=0.0, help='per-line sampling clock jitter (ns rms)')
    ap.add_argument('--edge', type=int, default=0, help='chroma suppression at luma edges 0-3')
    ap.add_argument('--diag', action='store_true', help='diagnostic view')
    ap.add_argument('--reffilt', type=int, default=0, help='reference filter over lines 0-1')
    ap.add_argument('--sharp', type=int, default=0, help='sharpness 0-7')
    ap.add_argument('--igain', type=int, default=0, help='reference ramp gain 0-3 (1x 1.5x 2x 3x)')
    ap.add_argument('--setup', type=float, default=0.0, help='NTSC setup (0.075 = 7.5 IRE)')
    ap.add_argument('--stripes', action='store_true', help='black/white stripes (cross-colour test)')
    ap.add_argument('--show', type=int, default=4, help='lines to show (from the end)')
    args = ap.parse_args()
    R, G, B, starts, st, N, pal = run(args)
    exp = [tuple(int(round(v * 255)) for v in b) for b in BARS]
    print('lines out: %d, last status %08x' % (len(starts), st[-1]))
    s = st[-1]
    corr = ((s >> 27) & 0x1f)
    corr = corr - 32 if corr & 0x10 else corr
    kk = (s >> 16) & 0xff
    print('status: burst %d blank %d black %+d kill %d alt %d corr_sat %d corr %d' % (
        s & 0xff, (s >> 8) & 0xff, kk - 256 if kk & 0x80 else kk, (s >> 24) & 1, (s >> 25) & 1, (s >> 26) & 1, corr))
    worst = 0
    for ln in range(len(starts) - 1 - args.show, len(starts) - 1):
        m = measure(R, G, B, starts, N, pal, ln)
        errs = [max(abs(a - e) for a, e in zip(mm[:3], ee)) for mm, ee in zip(m, exp)]
        worst = max(worst, max(errs))
        print('line %3d: ' % ln + ' '.join('%3.0f/%3.0f/%3.0f(%2.0f)' % mm for mm in m) + '  maxerr %.1f' % max(errs))
    print('expected: ' + ' '.join('%3d/%3d/%3d    ' % e for e in exp))
    print('WORST %.1f' % worst)
    # luma stability of the white bar over the lines (bands)
    wl = [measure(R, G, B, starts, N, pal, ln)[0][1] for ln in range(30, len(starts) - 1)]
    # horizontal jitter: position of the luma edges of each line (relative to
    # the output hsync), rms of the line to line deviation
    Yall = 0.299 * R + 0.587 * G + 0.114 * B
    fs_us = N / 64.0 if pal else N / 63.556
    act0_ = (10.5 if pal else 9.4); act1_ = (64.0 if pal else 63.556) - 1.5
    pos = []
    for ln in range(30, len(starts) - 1):
        s0_ = starts[ln]; row = []
        for i in (1, 3, 5, 7):
            c = s0_ + int((act0_ + i * (act1_ - act0_) / 8) * fs_us)
            seg = Yall[c - 24:c + 24].astype(float)
            lo, hi = seg[:6].mean(), seg[-6:].mean()
            if abs(hi - lo) < 15: continue
            mid = (lo + hi) / 2
            for k in range(1, len(seg)):
                if (seg[k - 1] - mid) * (seg[k] - mid) <= 0 and seg[k] != seg[k - 1]:
                    row.append(k - 1 + (mid - seg[k - 1]) / (seg[k] - seg[k - 1])); break
        if row: pos.append(np.mean(row))
    if pos:
        pos = np.array(pos)
        print('LINE JITTER: %.3f samples rms (%.1f ns)' % (pos.std(), pos.std() / 27e6 * 1e9 if args.xtal else pos.std() / (N * 15734.0) * 1e9))
        dd = np.diff(pos) / math.sqrt(2)
        print('LINE JITTER line to line: %.3f samples rms' % dd.std())
    # per-line colour error of the bars (as in the capture analysis): hue /
    # saturation of each bar against its mean over the lines
    if not args.diag:
        Ls = list(range(30, len(starts) - 1))
        if args.vblank:
            fl = 312 if pal else 262
            Ls = [l for l in Ls if (l % fl) >= 20]
        cm = []
        for ln in Ls:
            m = measure(R, G, B, starts, N, pal, ln)
            row = []
            for k in range(1, 7):
                r_, g_, b_ = m[k][:3]
                yy = 0.299 * r_ + 0.587 * g_ + 0.114 * b_
                row.append(0.492 * (b_ - yy) + 1j * 0.877 * (r_ - yy))
            cm.append(row)
        cm = np.array(cm)
        q = cm / cm.mean(axis=0)
        ph = np.degrees(np.angle(q))
        xs = np.arange(6) - 2.5
        sl = (ph * xs).sum(1) / (xs ** 2).sum()
        print('BAR ERR per line: phase %.2f deg  amp %.2f %%  (phase slope across bars %.2f deg/bar)' % (
            np.sqrt((ph ** 2).mean()), 100 * np.sqrt(((np.abs(q) - 1) ** 2).mean()), np.sqrt((sl ** 2).mean())))
    cy = np.array([measure(R, G, B, starts, N, pal, ln)[2][:3] for ln in range(30, len(starts) - 1)])
    print('CYAN line-to-line: std R %.2f G %.2f B %.2f  p-p B %.1f' % (cy[:, 0].std(), cy[:, 1].std(), cy[:, 2].std(), np.ptp(cy[:, 2])))
    print('WHITE G over lines: min %.1f max %.1f p-p %.1f' % (min(wl), max(wl), max(wl) - min(wl)))
    # luma / chroma alignment at the bar edges (positive: chroma later)
    ln = len(starts) - 3
    s0 = starts[ln]
    fs_us = N / 64.0 if pal else N / 63.556
    act0 = (10.5 if pal else 9.4)
    act1 = (64.0 if pal else 63.556) - 1.5
    Rf, Gf, Bf = R.astype(float), G.astype(float), B.astype(float)
    Yf = 0.299 * Rf + 0.587 * Gf + 0.114 * Bf
    Uf, Vf = (Bf - Yf) * 0.492, (Rf - Yf) * 0.877
    def cross(sig, a, b):
        seg = sig[a:b]
        lo, hi = seg[:10].mean(), seg[-10:].mean()
        if abs(hi - lo) < 15:
            return None
        mid = (lo + hi) / 2
        for k in range(1, len(seg)):
            if (seg[k - 1] - mid) * (seg[k] - mid) <= 0 and seg[k] != seg[k - 1]:
                return a + k - 1 + (mid - seg[k - 1]) / (seg[k] - seg[k - 1])
        return None
    dl = []
    for i in range(1, 8):
        c = s0 + int((act0 + i * (act1 - act0) / 8) * fs_us)
        ty = cross(Yf, c - 40, c + 40)
        tc = [t for t in (cross(Uf, c - 40, c + 40), cross(Vf, c - 40, c + 40)) if t is not None]
        if ty is not None and tc:
            dl.append(sum(tc) / len(tc) - ty)
    if os.environ.get('YDUMP'):
        c = s0 + int((act0 + 7 * (act1 - act0) / 8) * fs_us)
        print('Y blue->black: ' + ' '.join('%.0f' % v for v in Yf[c - 12:c + 12]))
    if dl:
        print('CHROMA-LUMA DELAY (samples): ' + ' '.join('%+.1f' % d for d in dl) + '  mean %+.2f' % (sum(dl) / len(dl)))
    if args.stripes:
        ln = len(starts) - 3
        s0 = starts[ln]
        a = s0 + int((10.5 if pal else 9.4) * N / 64.0) + 20
        b = s0 + int(60 * N / 64.0)
        r, g, bb = R[a:b].astype(int), G[a:b].astype(int), B[a:b].astype(int)
        cc = np.maximum(np.abs(r - g), np.maximum(np.abs(r - bb), np.abs(g - bb)))
        print('CROSS-COLOUR max %d mean %.1f' % (cc.max(), cc.mean()))


if __name__ == '__main__':
    main()
