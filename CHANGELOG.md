Changelog
=========

VHS firmware 0.14 (test)
------------------------

### Added
- VHS decoder > Mask left / right (pixels of the 720 wide picture, 0-64) and
  Mask top / bottom (lines of a field, 0-40): the picture borders shown black
  (the picture window of the scan converter, no FPGA change), the same part
  of the picture in View and Archive, PAL and NTSC; applied at once. The 720
  pixel window (53.3us) is wider than the 52us of a picture line: on the
  right it shows the line end, a few pixels before the next sync (a light /
  disturbed strip); at the bottom the head switch. Settings reset (four
  settings more).

VHS firmware 0.13 (test)
------------------------

### Fixed
- "Shadows": gone. The line timing now always comes from SOGOUT, the raw sync
  slicer of the TVP7002, instead of HSOUT of its sync processor (automatic
  HSYNC polarity detection, coast), which went wrong for a field now and
  then on VHS (the field shifted right by the sync width, the line start
  taken on the trailing edge, the burst in red at the left edge and the next
  field washed out; or ~100 lines skewed in streaks). Hardware test with
  0.12, same stretch of tape: 16 disturbed frames in 99 s with HSOUT, 0 in
  106 s with SOGOUT; 49 s of the same tape on a CRT: 0. SOGOUT leads HSOUT
  by 18 clocks (picture 9 pixels further right, measured): the regenerated
  hsync comes 18 clocks after the edge, so picture position, burst gate and
  clamp are as before.

### Removed
- The 0.12 test option Line sync (settings layout kept, no reset).

VHS firmware 0.12 (test)
------------------------

### Added
- Test option VHS advanced > Line sync: TVP7002 (HSOUT of the sync
  processor, as before) / SOG raw (SOGOUT, the raw sync slicer). The shadows
  are still there with 0.11 (same rate): not the vertical sync. In them the
  whole field is shifted right by ~5.3us (about the sync width: the line
  start taken on the trailing edge of the sync, the burst seen at the left
  edge in red, the next field washed out) or the top third of a field is in
  streaks (lines skewed, the line period lost for ~100 lines): the line
  timing from the TVP7002 sync processor (automatic HSYNC polarity
  detection, coast) going wrong for a field. SOG raw takes the line timing
  from the raw slicer instead, to compare on the hardware. Settings reset
  (one more setting).

VHS firmware 0.11 (test)
------------------------

### Fixed
- "Shadows" (a frame in streaks or with a coloured band at the left edge,
  the next one too dark / washed out; about every 8 s on a nearly new tape,
  the same stretch steady on a CRT): fields with a missing or spurious vsync
  from the TVP7002, the line count lost for that field and the decoder black
  level measured on the wrong lines with it. Vertical flywheel (output side):
  a field lasts as long as the one two fields before (312 / 313 lines, 262 /
  263 NTSC); when locked a vsync giving another length is ignored and a
  missing one is replaced at the expected line; a field begun that way with a
  vsync elsewhere in it, or two such vsyncs in a row (splice, search), unlock
  it and the next vsync is taken. The measured interlace flag changes after 4
  fields in a row read the other way (not by a single missing / spurious
  vsync). sim/fe `runfid.sh` with `VMISS` / `VEXTRA` (5% of the fields): 3
  placed off, 2 missed starts / 7 placed off, 5 extra starts -> 0 / 0 (PAL
  and NTSC); a splice: 2 fields off (0.9: 1 plus a progressive read).
- The field one line off in View captures with some dongles (MS2130):
  Frame lock step default 2 clocks (was 8); the dongle took the frame one
  line off when a blanking line was 8 clocks longer / shorter. Same lock
  margin on the hardware test (no resync, no lock loss).

### Changed
- Base: VHS 0.9 (frontend parity changes of 0.8 / 0.9 back), without the
  0.10 diagnostics.
- <VHS timing> shows only E (frame lock offset; "E lost" for a lock loss): the
  flywheel line length ranges J / A and the generated edges M removed for
  FPGA space.

VHS firmware 0.10 (diagnostics)
-------------------------------

### Added
- Diagnostics <VHS fields>: S = output frames whose bob parity repeated the
  previous one's (View: a field one line off), F = field parity readings
  read by the frontend with the parity of the previous one, I = output frames
  begun with the source interlace flag off; since boot, mod 256. The field
  one line off is still there with 0.9 (View captures: about once a second,
  the same field type, output resyncs R not moving): it is not the frontend
  parity reading. The bob parity of the scan converter is its own toggle
  (inverted every output frame, set at a resync, forced odd when the source
  interlace flag is off); sim/scan with the VHS model does not reproduce it.

### Changed
- The frontend parity changes of 0.8 / 0.9 are reverted (no effect on the
  hardware, the FPGA full: the diagnostics did not fit). F counts instead the
  fields the frontend reads with the parity of the previous one.

VHS firmware 0.9 (test)
-----------------------

### Fixed
- The field placed one line off was still there with 0.8 (a View capture:
  about once a second, the same signature). Besides the race of 0.8 the
  vsync edge of the TVP7002 can come on the wrong half line (on the last
  equalizing pulse instead of the first broad one), which no delay fixes.
  Now the fields of an interlaced source alternate: a parity read once
  against the alternation (equal to the previous reading, which itself
  differed from the one before) is taken as the expected parity, with the
  field start of that parity; a repeated one is real (a dropped field at a
  splice: one field off as before; a progressive source, e.g. VCR pause,
  stays progressive). Output side and measurement side (the interlace flag
  no longer drops for that field). The 1/16 line delay of 0.8 is kept.
  sim/fe `runfid.sh` with `EPE` / `EPO` (vsync half a line early) and `PROG`:
  5% of the even fields early, 50 fields: 0.8 2 placed off / 4 progressive
  reads -> 0 / 0; every other case unchanged (PAL and NTSC).

VHS firmware 0.8 (test)
-----------------------

### Fixed
- A field placed one line off every ~30 fields (View: the picture jumps one
  line for a field; Archive: the field one line off as well). The TVP7002
  vsync edge comes together with a sync edge of the composite sync: when the
  vsync edge was registered first, the field parity reference was the
  previous sync edge, half a line earlier, and the field was read as the
  other one with its start one line early (seen in a View capture: always the
  same field of the frame, its content one line off). The parity is now
  taken 1/16 line after the vsync edge (sync separated mode, output side and
  measurement side), when that edge is always in. The measured interlace
  flag no longer drops for such fields either. sim/fe `tb_fid.v` /
  `runfid.sh`: vsync edge racing the sync edge, 50 fields: 5 placed off and
  19 read as progressive -> 0 / 0 (PAL and NTSC); a splice as before.

VHS firmware 0.7 (test)
-----------------------

### Changed
- VCR search / fast forward: the picture no longer goes black ("Unsupported").
  The disturbed signal (noise bars, pseudo vsyncs) can make the frontend
  read a line rate far from 15kHz (e.g. 31kHz / 103Hz), taken as a new
  unsupported mode after 8 reads. Now a reading that is not a 15kHz signal
  (line rate outside 14.8-16kHz) is ignored: the current mode and output are
  kept and the FPGA frame lock follows what it can. Only the loss of the
  sync is a sync loss (before: also 8 invalid reads in a row). A real field
  rate / line count change in search (e.g. 604 lines at 51.5Hz) is still a
  mode change: a short picture loss, the HDMI output stays.

VHS firmware 0.6 (test)
-----------------------

### Changed
- Tape stop / start, source off / on: the HDMI output no longer drops. The
  output was on the TVP7002 clock (FPGA PLL bypassed) with a source and on
  the FPGA PLL from the 27MHz reference for the test pattern / safe screens,
  the PLL reference itself switched with the source, and every safe screen
  set the HDMI TX up again. Now the output is always on the FPGA PLL from the
  27MHz reference with one configuration (FPGA PLL BW from the settings), and
  the HDMI TX is set up again only when what it sends changes (CEA mode,
  pixel repetition, TX mode) or the PLL was reconfigured: only the picture
  changes (a capture keeps recording). One output resync when the source
  comes back (its phase is new).
- View: the sync / porch timings of the CEA preset (the line doubled 576i had
  a 6-line vsync / 38-line back porch, CEA 576p 5 / 39, and other hsync / back
  porch clocks), the same as the safe screens.

### Added
- Burst check: a burst more than 45 degrees away from the last good one is
  not used (the line takes the last good reference); after 6 such lines in a
  row the new phase is taken. Colorstripe copy protection (bands of 2-4 lines
  with the burst turned, broadcast / DVD outputs: not on VHS tapes) and
  bursts hit by tape dropouts. sim/cvbs `--cstripe 4`: hue error on the
  striped lines 72 degrees -> 0.3 (as without colorstripe).

### Simulation
- sim/cvbs: VHS timebase model with the crystal clock (`--vhs-ns`,
  `--vhs-period`, `--vhs-jit`: line starts moved, colour carrier continuous),
  colorstripe (`--cstripe`, `--csphase`); `runstd.sh`: NTSC, PAL, PAL-60,
  NTSC 4.43, PAL-M, PAL-N, clean and from a tape (+-3us drift, line jitter,
  chroma phase error 8 degrees, subcarrier +30ppm, noise), firmware defaults:
  every PAL standard as good as clean (delay line), NTSC / NTSC 4.43 with the
  constant hue error of the tape (no delay line in NTSC: Hue corrects it, as
  on a TV); horizontal jitter 25-100ns with +-3us of timebase error.

VHS firmware 0.5 (test)
-----------------------

### Added
- PAL delay line (VHS decoder menu, On by default): U / V averaged with the
  line before, as in a PAL TV (PAL, PAL-M, PAL-N, PAL-60; NTSC unchanged).
  A tape's chroma has a phase error against its burst; with V switched on
  alternate lines the hue turned one way on a line and the other way on the
  next (Hanover bars: purple / red lines). sim/cvbs `--cphase 10`: line to
  line hue error 8.8 degrees -> 0.2 (as without error); halves the chroma
  noise too.

### Fixed
- Tape splice (a field dropped): two output resyncs, the TV blank twice. The
  splice gives one odd frame, then the new phase; both resynced the output.
  Now an input frame start out of lock resyncs only when the previous one
  was out of lock at the same output line (new phase confirmed): the odd
  frame is ignored (picture wrong for a frame or two, HDMI timing unchanged),
  one resync. sim/scan (`SPF` / `SPC`: splice frame / jump): one resync for a
  dropped field, picture clean after it.
- Soft lock window half an input line for every output (0.1-0.4: half an
  output line, a quarter of an input line in View): jumps up to half an
  input line (~32us) now absorbed without a resync in View too.

VHS firmware 0.4 (test)
-----------------------

### Fixed
- Setup redone at a damaged stretch / splice of a tape (always at the same
  point; same mode before and after, the TV black for seconds): a mode change
  counted after 3 differing status reads, whatever they read, and a single
  read without a valid mode was a sync loss (then a setup at the sync up).
  Now the new mode must be read the same 8 times in a row (~160ms), and an
  invalid mode with the sync active is a loss only after 8 reads in a row
  (no sync at all from the frontend: at once, as before). Same for every
  source. The output frame lock still resyncs there when the field timing
  jumps (no frame memory: a TV fed by the VCR rolls there).

VHS firmware 0.3 (test)
-----------------------

### Fixed
- Jagged vertical edges on tapes (`<VHS timing>` M31: 31+ generated syncs per
  field): the hsync flywheel accepted an edge from 7/8 of the line but
  generated a missing one only 4 clocks after the expected end; a tape's
  lines are longer / shorter than the previous one by more than that, so
  most lines got a generated edge and their real one was dropped (lines out
  of place). Now generated 1/8 line late, the late side of the window as wide
  as the early one (sim/fw: line to line variation of +-8 clocks, 2943 of
  2951 lines generated before, none now; real missing syncs still made up;
  Macrovision pseudo syncs ignored).
- Black level pumping on some original tapes: the blank level was measured
  on every line, also in the vertical blanking where Macrovision puts its AGC
  pulses (white level, cycling). Now only the lines 24-239 after the vsync,
  for every source.

### Diagnostics
- `<VHS events>`: B = range of the blank level over the picture lines of a
  field (ADC codes, largest of 12 fields), P = setups since boot, R = output
  resyncs since boot (mod 256). A black screen on the TV with P up: setup
  redone; with R up: frame lock resync.

VHS firmware 0.2 (test)
-----------------------

### Fixed
- Original (copy protected) tapes: the OSSC kept redoing the setup, line
  counts all over the place (~220-1160 lines per field), "Unsupported", 50 /
  100Hz. The frontend measured the line period once per field on the first
  sync edge 1ms after the vsync (line ~16): right in the Macrovision AGC
  pulse lines, where it got the spacing of the pseudo syncs (~216 clocks
  instead of 1728); the line count and the interlace detection built on it
  went wrong (reproduced in sim/fe, PAL and NTSC). Now, the same rules for
  every source:
  - line period measured 8ms after the vsync (mid field), from two
    consecutive lines of the same length (+-8 clocks);
  - a new line only from 7/8 of the line (as the flywheel); edges before
    that which are not half-line pulses are ignored;
  - in the vertical blanking a missing line is made up after 9/8 of the line
    (stock: 4 clocks: a tape's line jitter made up lines and dropped the real
    edge right after them, losing the field parity).
- Mode changes: seen in 3 consecutive status reads (a single odd field no
  longer redoes the setup), field period tolerance 1% (was 50 clocks), sync
  width tolerance 16 (was 8).

### Simulation
- sim/fe: frontend measurement testbench (composite sync with equalizing /
  broad pulses, PAL / NTSC, Macrovision pseudo syncs, VHS model): 625 / 525
  interlaced on every field with and without Macrovision and tape timebase
  errors (stock: 385-424 lines, progressive, with Macrovision).

VHS firmware 0.1 (test)
-----------------------

A firmware of its own, derived from the CVBS decoder 1.0.2, to watch and
digitize VHS tapes from a VCR over HDMI (TV or USB capture card). Composite
and S-Video on AV2, NTSC and PAL alike. HDMI only (no 15kHz output).

### Output
- Output: Archive (480i / 576i, CEA 1440x480i / 1440x576i with pixel
  repetition: the fields as they are, for deinterlacing later in software)
  or View (480p / 576p, bob). Always on the 27MHz clock with the exact CEA
  line length (1716 / 1728, View 858 / 864): no FPGA PLL following the
  source (CVBS 0.46-1.0).
- Soft frame lock both ways (scanconverter): the input frame period (tape,
  VCR crystal) a little longer or shorter than the output one is made up in
  the back porch lines, up to 1 << n clocks per line ("Frame lock step" in VHS
  advanced, default 8). The picture lines, the line count and the vsync never
  change; a resync only at lock / a jump of half a line or more. (CVBS: one
  way only, output lines rounded up.)
- Line2x (View): 4-line buffer again (as in 0.45, same RAM): with 2 lines
  the bob output had almost no room for the lock (torn lines in sim/scan).
- Bob lock: the source field parity check assumed the frame start at line 0
  (Line2x starts at line 523: no soft lock was ever accepted there).
- Test pattern / safe screens in the CEA mode of the selected Output and of
  the last colour system (480i / 576i / 480p / 576p), not 240p.
- No setup redone for small line period changes (a tape wanders): only when
  the input line length is off by more than 1.5%.

### Removed (space, HDMI only)
- 15kHz output, Line3x, Line4x, the 15kHz watchdog.
- CSync on HSYNC (XOR / AND / Standard) and the composite sync generator.
- Line-locked sampling (always the 27MHz crystal).
- Adaptive luma notch (plain notch only: decoder output identical to 1.0.2
  with "Simple", sim/cvbs NTSC and PAL), Edge color sup.

### Diagnostics
- `<VHS timing>` (VHS advanced > Diagnostics), over ~1s: J = range of the
  line lengths in the middle of the field (27MHz clocks, sync edge to sync
  edge), A = the same over the whole field (head switch included), E = frame
  lock offset at the input frame start (clocks, R = resync), M = missing
  syncs per field (generated by the flywheel).

### Settings
- Own settings ("OSVH" v1): the CVBS settings are not loaded.

### Simulation
- sim/scan/runvhs.sh: VHS model in tb_lb.v (slow drift, head switch, line
  jitter, frame start jitter), 525 / 625 lines, Archive / View, +-100 and
  300ppm: no torn lines, one resync (lock) up to +-100ppm, picture lines
  always 1716 / 1728 / 858 / 864, vsync parity right.

CVBS decoder 1.0.2 (test)
-------------------------

### Added
- Adaptive luma notch for PAL (PAL, PAL-M, PAL-N, PAL-60): the line compared
  is the one two lines before (PAL: V switched on alternate lines, the
  colour repeats every two lines; the luma detail turns by 2 x 270.6 = 181
  degrees and cancels). Simulation (sine of 3.5MHz in the luma, line-locked
  and crystal clock): fine luma detail kept 77-86% (plain notch 14-35%, no
  notch 84%), colour bars unchanged.
- sim/cvbs: --fine (luma detail test), --simple.

CVBS decoder 1.0.1 (test)
-------------------------

### Changed
- FPGA remote update IP removed (~250 logic elements): this firmware never
  used it (no firmware update of its own, updates are done from the official
  firmware; "Launch 2nd FW" uses the official firmware's block). The hard
  block alone is still instantiated (no logic): the project keeps the remote
  configuration mode of 1.0.

CVBS decoder 1.0
----------------

First release: 0.47 renamed, no other change.

CVBS decoder 0.47
-----------------

### Fixed
- HDMI TVs at random without picture (screen going on and off) after a
  source change in Line2x (e.g. Wii 240p -> 480i: output clock +1.3%): the
  HDMI TX was reset while the FPGA PLL was still relocking. 20ms wait after
  a PLL reconfiguration.

CVBS decoder 0.46
-----------------

### Changed
- 27MHz crystal clock: the output clock is on the FPGA PLL, 27MHz x M/(N*C)
  with the output line length chosen so that the output follows the source
  line rate: the output lag left per frame is ~15-35 clocks (0.44: SNES
  ~340, Wii 480i ~515), made up by the soft frame lock of 0.44 (0.45
  reverted: frames with a varying number of lines, some HDMI TVs lose the
  picture; 15kHz CRTs: picture jumps) with at most 8 clocks per back porch
  line (0.44: 64; sensitive 15kHz CRTs: bent top of the picture). Output
  clock within 1% of 27MHz (40.5 / 54MHz in Line3x / 4x), output line
  length within 2% of the preset. <Xtal timing> shows the lag per frame.
- Line2x / 3x / 4x of 480i / 576i sources (bob): a frame lock found without
  a resync (e.g. at power-up) could have the other source field parity
  (torn lines, see sim/scan): the lock now also checks it.

CVBS decoder 0.45 (not released)
--------------------------------

### Fixed
- 27MHz crystal clock, 15kHz CRTs: bent top of the picture with 0.44 (the
  shortened back porch lines disturbed the horizontal flywheel). Frame lock
  with whole lines: all output lines (blanking included) have the same
  length, the nearest whole number of clocks to the source line; at every
  input frame start the output position is checked and, when it is more
  than half a line off, the next front porch gets one line more or one line
  less. The picture keeps its place after the vsync and the vsync its place
  in the line (480i / 576i field parity). E.g. Wii 480i: one frame of 524 /
  526 lines every ~3s; SNES 240p: one frame of 261 / 263 lines every ~0.4s.
- Line2x / 3x / 4x of 480i / 576i sources (bob): a lock found without a
  resync (e.g. at power-up) could have the other source field parity (torn
  lines). The lock now also checks it.

### Changed
- Line2x / 3x / 4x: 4-line buffer (input lines of 720 samples) and the
  output frame starts one input line later: these modes read a line 2-4
  times faster than it is written, 2 lines left too little room for the
  frame lock.
- sim/scan: line buffer simulation (tb_lb.v, anlb.py, runall.sh): which
  input line every output picture line shows, for 15kHz and Line2x-4x
  sources and line periods, worst cases included.

CVBS decoder 0.44
-----------------

### Fixed
- Interlaced sources (480i / 576i) with the 27MHz crystal clock on 15kHz
  CRTs: 0.43 still showed the two fields on the same lines (the CRT's
  horizontal flywheel was still off after the line cut short, 4 lines before
  the vsync). Soft frame lock: when the input frame starts within the last
  2 lines of the output frame, there is no resync any more; the output
  finishes its frame and the lag (up to ~a clock per line of the frame) is
  made up in the next frame, by shortening the back porch lines after the
  vsync by up to 64 clocks each (half in each field of interlaced modes),
  stopping 3 lines before the picture. No line cut short before a vsync,
  the picture lines all the same length, the output never ahead of the
  input; same rule for every mode and output. A resync (line cut) only to
  lock, at the start or when the source changes. The vsync is back at its
  standard position (0.29-0.43: 4 lines later with the crystal clock, to
  keep it away from the line cut short by the resync: no such line now).
- Simulated before the build (`sim/scan`: scanconverter timing with
  480i / 576i / 240p / 288p and Line2x sources, input jitter): one resync at
  the start, then the vsync at a line start / half a line in alternate
  fields, equal field lengths, all picture lines and the lines before the
  vsync of the same length.

### Changed
- Sync diag.: `S` counts the resyncs (line cuts) only: 0 per second once
  locked (was one per input frame).

CVBS decoder 0.43
-----------------

### Fixed
- Interlaced sources (480i / 576i) with the 27MHz crystal clock on 15kHz
  CRTs (fields swapped or no picture), without the line length fraction of
  0.41 / 0.42 (an HDMI TV shifted the lines one clock longer: wobbly
  outlines). Whole output lines on every output as in 0.39; the vsync now
  starts 4 lines after the resync point in interlaced modes too, as it
  already did in progressive ones since 0.29 (one rule for all modes): the
  line cut short by the resync is no longer right before the vsync of the
  first field, there are full lines between them. Both fields' vsyncs move
  by the same 4 lines (the second field keeps its half-line offset).
- Xtal timing: `O` back to the whole output line length.

CVBS decoder 0.42
-----------------

### Changed
- Output line fraction (0.41) on the 15kHz output only. Line2x / 3x / 4x:
  whole lines as before (an HDMI TV shifted the lines one clock longer by a
  pixel: wobbly outlines on the MiSTer menu and Mega Drive; the interlaced
  fields are line doubled there, the resync cut is harmless).

CVBS decoder 0.41
-----------------

### Fixed
- Interlaced sources (480i / 576i) with the 27MHz crystal clock on 15kHz
  CRTs: fields swapped (Standard csync) or no picture (other sync types).
  The output line was a whole number of clocks, up to one clock longer than
  the source line (Wii 480i: 1716.0 clocks per line, output 1717), and the
  resync at every input frame cut the last line short by the difference
  accumulated over the frame (~525 clocks, a third of a line, right before
  the vsync of the first field: the CRT could not tell the fields apart).
  Now the output line has a fraction: lines of N clocks, one clock longer at
  the rate of the fraction (1/256 clock steps, from the measured line period,
  updated while it runs, +2/256 so that the output never gets ahead of the
  source). The resync cuts only a few clocks. The active picture of every
  line is unchanged (the extra clock is in the horizontal blanking). Sources
  with lines shorter than the preset (SNES ~1714.7 clocks) keep the preset.
- Xtal timing: `O` shows the output line length with its fraction
  (O1716.03).

### Removed
- Event count.: the `X` counter (resyncs out of place), logic space.
- (0.40 test: vsync from the composite sync with the crystal clock: no
  sync; not the cause, removed again.)

CVBS decoder 0.39
-----------------

### Changed
- Edge color sup.: default Low (was Medium, a bit too strong).
- The 2nd button on the OSSC has no function (it changed settings of the
  Downscaler firmware, unused here, and redid the mode setup).
- README rewritten with the structure of the Downscaler / Downscaler HD ones.

CVBS decoder 0.38
-----------------

### Changed
- Luma notch Adaptive: 3 lines with NES / SNES. Their subcarrier turns by
  120 degrees per line (227.33 cycles per line), so the average of two lines
  only halved the luma detail; the sum of three lines cancels it (1 + e^j120
  + e^j240 = 0). The firmware picks 2 or 3 lines from the measured field
  period (`3` at the end of Decoder diag.). Three lines of similar colour
  magnitude: average of three; else the 2-line rule; else the plain notch.
  Second chroma line memory (Z of two lines before).
- Decoder diag.: the `K` black level readout of 0.37 is gone (logic space
  for the 3-line notch).

CVBS decoder 0.37
-----------------

### Added
- NTSC 7.5 IRE: Auto (default), Off, On. NTSC-M (and PAL-M) put the black
  7.5 IRE above the blank level; the decoder took the blank level as black,
  so blacks came out grey (brightness -6 needed to match RGB). On: the setup
  is removed (offset, gain x100/92.5: white stays white). Auto: on with
  NTSC, NTSC 4.43 and PAL-M (the system can not tell NTSC-J, which has no
  setup).
- Decoder diag.: `K±n`, the darkest luma of the picture in ADC codes above
  the blank level (minimum of 16-sample averages in the active area of the
  last field): with a black screen it shows the black level of the source.

### Changed
- Black level: back to the level in the burst gate (0.36 measured it in the
  back porch too: the two read the same, no sync recovery offset).

CVBS decoder 0.36
-----------------

### Changed
- Luma notch: Adaptive (new default), Off, Simple. The notch removes the
  colour from the luma around the subcarrier frequency, and with it the
  luma detail there: thin outlines of NES / SNES / MiSTer pixel art got
  blurred, with a dark grey ghost pixel beside them. Adaptive (NTSC): the
  colour estimate is compared with the line above at the same position;
  the colour of the source is the same from line to line, luma detail is
  not (its phase turns by 180 degrees per line, 120 on NES / SNES): with
  similar magnitudes the two lines are averaged, the luma detail cancels
  (halved on NES / SNES) and stays in the luma. Horizontal edges between
  colours, and PAL: the plain notch (Simple). No extra line memory.
- Black level: measured in the back porch after the clamp, just before the
  picture, instead of during the burst gate. On real sources the level
  ~1.5us after the sync was ~7-8 ADC codes below the black of the picture
  (brightness -6 was needed to match RGB, also with S-Video); the
  simulation showed no offset. The burst gate level is still used when the
  two differ by more than 16 codes (picture or border in the back porch).
- Decoder diag.: `L` shows both levels (burst gate / back porch) instead of
  the peak code.

CVBS decoder 0.35
-----------------

### Changed
- Output: 15kHz, Line2x 31kHz, Line3x 47kHz, Line4x 63kHz. Line2x is the
  CEA 720x480p / 720x576p at 27MHz of 0.32-0.34 "Line2x 720"; the 1440 wide
  Line2x at 54MHz is gone (no visible difference with composite / S-Video, a
  less standard signal). Saved settings of 0.34 are converted (settings v5).
- The test pattern / NO SYNC / Unsupported screens are shown at the selected
  output (480p, 786 or 1048 lines) instead of 240p, so that an HDMI TV is not
  switched to 240p when the source is off; shown again when Output changes.
- Inputs: AV2 composite and AV2 S-Video only (remote keys 5 / 2, the input
  button toggles them); AV1 / AV3 and the input autodetection are gone (the
  autodetection could not tell composite from S-Video on the same Y input).
  Initial input: AV2 Composite (default), AV2 S-Video or Last used.

CVBS decoder 0.34
-----------------

### New
- Output > Line4x 63kHz: as Line3x, each line shown 4 times, output at 54MHz
  (2x the ADC clock), 858 clocks per line, 1048 / 1248 lines (960 / 1152
  active). Not a CEA mode.

### Changed
- Output is saved with the settings (was a test option, 15kHz at every
  boot). "Line2x 31kHz" is now "Line2x 1440".
- Menu: the technical decoder items moved to a new main menu entry "CVBS
  advanced" (ADC gain / offset, burst / clamp position, sample clock, line
  re-timing, ref. filter, Adv. timing, Diagnostics submenu with Decoder diag.,
  Sync diag., Xtal timing, Event count.). "CVBS decoder" keeps the user
  settings.

CVBS decoder 0.33
-----------------

### New
- Output > Line3x 47kHz: each line shown 3 times, one sample out of 2
  (858 / 864 per line, as Line2x 720), output on the FPGA PLL at 3/2 of the
  27MHz ADC clock: 40.5MHz, 858 clocks per output line, 786 / 936 lines
  (720 / 864 active). Not a CEA mode, like the stock Line3x (accepted by the
  Samsung TV that drops frames in Line2x). Within the existing FPGA timing
  constraints (output PLL up to 66MHz).

CVBS decoder 0.32
-----------------

### New
- Output > Line2x 720: 720x480p / 720x576p at 27MHz exactly as in CEA-861
  (VIC 2/3, 17/18, no pixel repetition), one sample out of 2. With the 27MHz
  crystal the output clock is the crystal itself (FPGA PLL bypassed, unless
  FPGA PLL BW Low). Line2x 31kHz stays as before: 1440 wide at 54MHz through
  the FPGA PLL, sent as 480p with pixel repetition. For HDMI TVs that drop a
  frame now and then in Line2x (a Samsung TV here, also with the stock
  firmware in Line2x; stable in Line3x / Line5x).

### Removed
- The resync cut spread of 0.31 (Xtal frame end): the random black frames
  were the TV, also with line-locked sampling.

CVBS decoder 0.31
-----------------

### Changed
- 27MHz xtal clock: the resync cut is spread over the front porch lines.
  The output frame is a bit longer than the input one (output lines never
  shorter than the input ones), so the resync at every input frame cut the
  last line short: by up to half a line or more, depending on the source.
  HDMI TVs dropped a frame now and then (random black frame, about once a
  minute; never with line-locked sampling, never on a PC monitor). Now the
  front porch lines (no picture, no line buffer reads) are shortened a little
  each, adjusted at every resync so that only ~48 clocks are left to cut:
  e.g. Line2x 480p, 9 lines ~4% shorter instead of one line ~40% shorter.
  Progressive input and output only.
- Xtal frame end (CVBS decoder menu, test option, not saved): Spread
  (default) / Cut (as 0.29-0.30).

CVBS decoder 0.30
-----------------

### New
- Sharpness (Off, 1-7, CVBS decoder menu): luma peaking, as in 0.6-0.11.
  7-tap filter, no gain at DC and at the Nyquist frequency (no noise boost).
- CSync on HSYNC > Standard is back (equalizing pulses, serrated vsync), with
  the vsync offset of 0.29 in the crystal clock resync mode.

### Changed
- Sample clock default: 27MHz xtal (saved settings keep their value).
- Xtal timing: "L<line period> O<output line length>", or "L.. O..>.." when
  the applied length is outside the tolerance (the setup will be redone).
- Sync diag.: O F S (the frontend count f of 0.27 is gone).
- New <Event count.> (since boot): P = setups, U = register updates (also
  at every setup and settings change), X = output resyncs out of place (not
  in the last output line, or in its first 64 clocks: a very short line).
  To find out what happens at the random black frames on HDMI TVs.

### Removed
- Introduced for causes that turned out to be wrong (the real one, 0.29: a
  vsync right after the cut-short line, with separate H / V syncs):
  - Xtal vsync / own vsync from the composite sync (0.24): the TVP7002 vsync
    was in the right place (Sync diag. of 0.26).
  - Unlock of the hsync flywheel on generated edges (0.26); the flywheel is
    still restarted at every setup.
  - Xtal out sync (Resync / Off test options): always Auto.
- Test patterns Grid, Monoscope and RGB (the stock gray pattern stays, for
  the safe screens); shadow mask, border colour / brightness and Panasonic
  hack paths (no menu in this firmware): room for the above.

CVBS decoder 0.29
-----------------

### Fixed
- 27MHz xtal clock with separate H / V syncs (CSync on HSYNC Off, needed by
  HDMI TVs): picture moved or stretched at random on some sources (SNES, PC
  Engine), fine with csync (AND). The output is resynced at every input
  frame, cutting short the line before the frame start; the vsync started
  right after it, a fraction of a line after the previous hsync (depending on
  the source line period, up to ~half a line: SNES ~0.4, PC Engine ~0.55), so
  HDMI sinks could take it as an interlaced field. The vsync now starts 4
  lines later (progressive output, resync mode only): only full lines before
  it. The short line stays in the vertical blanking.

CVBS decoder 0.28
-----------------

### Fixed
- Sync diag. read all zeros and Xtal timing "Not stable" after 159s of
  uptime: the 64-bit timestamp minus a 32-bit start (every wait in these
  diagnostics ended at once, since 0.20). The readings of 0.24-0.27 taken
  after that (F0 S0) were not real. The same bug made the stable line period
  measurement of 0.27 fall back to a single field period after 159s.

CVBS decoder 0.27
-----------------

### Fixed
- 27MHz xtal clock, Xtal out sync Auto: picture moved, stretched or garbled
  at random after loading some MiSTer cores (SNES, PC Engine), depending on
  when the setup happened (e.g. with the OSSC menu open or not), until Sample
  clock was changed. The output line length came from the single field
  period that triggered the setup, possibly a transient one while the core
  was starting, and the mode detection tolerance (50 clocks per field) never
  redid it. Now the line period is the average of 4 stable fields, and it is
  checked continuously (16 samples, ~0.3s): the setup is redone when the
  applied line length is too short or longer than needed by more than 1/4
  clock per line (hysteresis for line periods at a rounding step, e.g. PC
  Engine ~1716.2 clocks).

### Changed
- Sync diag.: O = output frames, F / f = input frames at the decoder /
  frontend, S = output resyncs (per second). 0.24-0.26 read F0 S0 even with a
  good picture; counters now read with explicit shifts, and the output /
  frontend counts as a cross-check. The own vsync offset (V) and the sync
  width (W, ~115 clocks: fine) are gone.
- New <Xtal timing>: line period now (27MHz clocks per line), output line
  length applied / right for it now (`*`: outside the tolerance).

CVBS decoder 0.26
-----------------

### Fixed
- 27MHz xtal clock: picture moved down, black or stretched at random after a
  core / menu change, staying so (also back in the menu) until Sample clock
  was changed. The hsync flywheel (0.15) could lock to a wrong line length
  while the source was switching, and stay locked (a generated edge between
  two real ones); the mode detection did not see it (it measures the raw
  sync), changing Sample clock reset the flywheel. Now unlocked when more
  than 8 of 32 lines end on a generated edge, and restarted at every setup.
  Sync diag. in that state: F0 S0 (no frame starts): the own vsync (0.24)
  took its broad pulse threshold from the flywheel line length (below the
  hsync width): now fixed lengths (300 clocks, 1.25 lines).
- Xtal vsync default back to TVP7002: with the own vsync Sync diag. read F0
  S0 (no frame starts) even with a good picture. Own kept as a test option;
  Sync diag. W = sync pulse width (27MHz clocks) to check its threshold.

### Removed
- The repeated setup of 0.25 (no effect).

CVBS decoder 0.25
-----------------

### Fixed
- 27MHz xtal clock: picture moved down at random after loading a MiSTer
  core, or black after a core / menu change, until the next setup (the own
  vsync of 0.24 did not change it). The setup is now repeated once, 1.5s
  after a mode change, with a full TVP7002 sync processing reset - what
  changing Sample clock did by hand. Not the root cause, which is still
  unknown.

CVBS decoder 0.24
-----------------

### Fixed
- 27MHz xtal clock: picture moved down by a large amount at random (e.g.
  after loading a MiSTer core), until the next setup; the output resynced
  every frame (Sync diag. F60 S60) but at the wrong line. The vsync output
  of the TVP7002 can lock at a wrong line without the H-PLL: the vertical
  sync is now taken from the composite sync, as in a TV (broad pulses: a
  sync longer than 1/8 line), keeping the half-line offset of interlaced
  fields.

### New
- CVBS decoder > Xtal vsync (Own, TVP7002; test, not kept, default Own).
- Sync diag.: V = lines from the own vsync to the TVP7002 one (x8); the
  flywheel lock flag is gone.

CVBS decoder 0.23
-----------------

### Fixed
- 27MHz xtal clock, Xtal out sync Auto: interlaced sources (480i) with the
  fields out of place, OSD garbled. The line period was computed with 262
  lines per field instead of 262.5: output lines 0.2% too long, the second
  field (not resynced) started about a line late.

CVBS decoder 0.22
-----------------

### Changed
- 27MHz xtal clock, Xtal out sync (Auto, Resync, Off; test, not kept):
  Auto = resync at every input frame + output line length from the measured
  line period, rounded up and never below the preset. Tests of 0.20 / 0.21
  on the monitor: Mega Drive and MiSTer menu fine with the longer line
  (Both), SNES fine only with the preset length (Resync): Auto does each.

### Removed
- Line lock of 0.21: no picture on any source (output line lengths changing
  from line to line, presumably not accepted by the monitor).

CVBS decoder 0.21
-----------------

### New
- 27MHz xtal clock: line lock (CVBS decoder > Xtal out sync = Line lock, the
  new default). Tests of 0.20: the output must never run ahead of the input
  (cut line) and must be resynced at every frame (periodic jumps: no
  picture), and no fixed output line length suits every source (SNES ~1714.8
  clocks per line, MiSTer menu ~1716, Mega Drive ~1719.8). Now the output
  position is checked at every input line and the difference (a few clocks)
  is taken out of the next output line, in its front porch: the output
  follows the input line by line, whatever the line length.
- Xtal out sync: Line lock, Both (line length rounded up + resync), Resync,
  Off. Line len. alone removed (periodic jumps by design).

### Removed
- Unverified changes, for room: vsync debounce (0.19), no frequency loop /
  reference filter update in the vertical interval (0.15), R in Sync diag.
- CSync on HSYNC > Standard (equalising pulses, serrated vsync), for room:
  Off (H+V), XOR and AND remain. Default now Off (H+V), as in the HD
  firmware (HDMI TVs); XOR / AND for SCART.

CVBS decoder 0.20
-----------------

### Fixed
- 27MHz xtal clock, Xtal out sync Line len. / Both: no picture with the
  SNES (MiSTer). The output line length was rounded to the nearest clock:
  shorter than the input lines on sources below 1716 clocks per line, the
  output read lines before they were written. Now rounded up.

### Changed
- Line re-timing reference back to an average over 64 lines (the 4000 lines
  of 0.17 were not verified, possibly the extra shaking on the Mega Drive).
- Sample clock default back to Line-locked (saved settings keep their value)
  until the crystal clock works on every source.
- Sync diag.: F = input frames per second, S = output resyncs per second,
  R = hsync edges ignored, L = flywheel locked.

CVBS decoder 0.19
-----------------

### Fixed
- 27MHz xtal clock: picture garbled on the MiSTer menu with 0.17 (the cores
  were fine with H+V sync, and much steadier). The TVP7002 vsync output can
  toggle more than once in the vertical interval without the H-PLL, and
  0.17 resynced the output at every vsync edge. The vsync is now debounced
  (a new level is taken after one whole line).

### New
- CVBS decoder > Xtal out sync (Both, Resync, Line len., Off; test, not
  kept, default Both): the two output timing changes of 0.17, resync at
  every input frame and line lengths from the measured period.
- Sync diag.: V = raw vsync position (clocks), R = hsync edges ignored, E =
  extra raw vsync changes per frame, L = flywheel locked.

CVBS decoder 0.17
-----------------

### Fixed
- 27MHz xtal clock: MiSTer menu black on the Line2x output, last line cut
  short on the Mega Drive. The cause was the output timing, not the sync:
  with the crystal a line is not a whole number of clocks and the output
  clock is not locked to the source, so the free-running output frame
  drifted against the input (periodic jumps when the drift went past the
  resync window: no lock on the monitor; line buffer read late at the
  bottom). Line lengths of the frontend and of the output now follow the
  measured line period, and the output is resynced at every input frame.
- 27MHz xtal clock: slow horizontal waviness. The line start quantisation
  drifts slowly through the sample on sources close to 1716 clocks per line
  (MiSTer), and the line re-timing reference (average edge over 64 lines)
  followed it: now averaged over ~4000 lines.

### Removed
- Vsync re-alignment of 0.16 (the vsync position was fine: Sync diag.).

CVBS decoder 0.16
-----------------

### Fixed
- 27MHz xtal clock: the TVP7002 vsync output also comes anywhere in the
  line without the H-PLL, and the frontend takes the position of the vsync
  edge in the line for field detection / frame start. The vsync is now
  re-aligned to the line start (or to the half line, when it comes in the
  middle half: interlaced sources). Hsync flywheel window narrowed to 7/8
  of the line.

### New
- CVBS decoder > Sync diag.: raw vsync position / clocks per line, hsync
  edges ignored by the flywheel in the last field, L = flywheel locked.

CVBS decoder 0.15
-----------------

### Fixed
- 27MHz xtal clock: black screen on the MiSTer menu, last line cut short
  and flickering line at the bottom on the Mega Drive. Without the H-PLL the
  TVP7002 hsync output also carries the half-line pulses of the vertical
  interval: a flywheel now ignores hsync edges before 3/4 of the line and
  fills in missing ones (like a TV).
- 27MHz xtal clock: slow hue wobble, larger at the top of the picture
  (captures of 0.14: ~1 level rms on cyan / red, the line to line streaks are
  gone). The frequency loop and the reference filter no longer update in the
  first lines after the vsync (no or odd bursts there).

### Changed
- All multipliers on the FPGA embedded multipliers (logic elements freed).

CVBS decoder 0.14
-----------------

Decoder rebuilt around the 27MHz crystal clock, like a TV: steady sampling
clock, picture timed on the sync of every line.

### New
- CVBS decoder > Line re-timing (On / Off): with the 27MHz xtal clock the
  start of each line is quantised to a sample, so every line was shifted by
  up to +-1 sample at random (11ns rms in the captures: shaky picture). The
  sync trailing edge of each line is now timed to 1/256 sample (area of the
  edge over 16 samples) and the luma is read from the line memory at that
  fractional position (chroma: nearest sample). Simulation: 11 -> ~4ns
  (S-Video, with noise). Only with the xtal clock.
- CVBS decoder > Ref. filter (Auto, Off, On): the colour reference is
  low-passed over the lines (1/8: less line to line hue / saturation noise
  from the burst measurement). Auto: On with the xtal clock, Off
  line-locked (there the reference moves from line to line).

### Changed
- Sample clock default: 27MHz xtal (saved settings keep their value).
- Frequency loop back (sign-only, fast acquisition): the DDS follows the
  subcarrier of the source, so with the crystal the burst phase is steady
  from line to line.

### Removed
- Ref. interp. and Interp. gain (reference ramp along the line, for the
  line-locked clock), Diag. view: room for the re-timing.

CVBS decoder 0.13
-----------------

### New
- CVBS decoder > Interp. gain (1x, 1.5x, 2x, 3x, test): strength of the
  reference ramp along the line. Captures of 0.12: the hue error still grows
  along the line (1.4 -> 2.6 degrees left to right, 1.9 -> 3.75 without the
  ramp, ~1 with the 27MHz crystal clock): the next burst sees only part of
  the H-PLL drift, which the PLL corrects in part at the hsync.

CVBS decoder 0.12
-----------------

### Fixed
- Horizontal colour streaks (saturation / hue changing from line to line,
  most visible on saturated colours). Measured with captures: the colour
  phase drifted along each line by ~2 degrees rms, a different amount on
  every line - the TVP7002 H-PLL sampling clock changes its frequency at
  every hsync. The active video is now demodulated one line later (line
  memories), and the colour reference is interpolated along the line
  between the burst of that line and the burst of the next one, following
  the drift (simulation: 1.6 -> 0.3 degrees). The picture comes out one line
  lower.
- Burst position default 165 (less line to line saturation noise than 180
  on the MiSTer + AD723).

### Changed
- CVBS decoder > Ref. interp. (On / Off, test) replaces Subcarrier loop:
  the interpolated reference also follows the slow rotation that the
  frequency loop corrected (the loop made no difference in the captures).

### Removed
- Sharpness (room for the line memories).

CVBS decoder 0.11
-----------------

### New
- Diag. view (CVBS decoder menu, not kept): "Color dev. x8" replaces the
  picture with the colour deviation from its own average over the lines
  (per group of 8 samples), amplified 8x on mid grey. A steady colour is
  flat grey, whatever the colour; line to line colour changes show as
  stripes. For tests and captures.

CVBS decoder 0.10
-----------------

### New
- CVBS decoder > Adv. timing (from the stock firmware): per mode (pick it
  with left / right on the item) samples per line, sampling phase, timings.
  The colour streaks depend on the sampling phase (with the 27MHz xtal clock
  they come and go as the phase drifts). Not kept across power cycles yet.
- Settings opt. > Phase hotkey (stock firmware): the remote phase +/- keys
  change the sampling phase of the current mode.

CVBS decoder 0.9
----------------

### New
- Sample clock (test): Line-locked (TVP7002 H-PLL, as before) or 27MHz xtal
  (the OSSC 27MHz reference on the TVP7002 EXT_CLK input as ADC clock). The
  H-PLL clock jitters against the colour subcarrier inside the line
  (streaks of lower saturation on saturated colours); the crystal does not.
  Not locked to the source lines: edges may move by up to one sample.
- Burst position default 180 (the burst level peaks there on the tested
  sources).

### Removed
- Ref. filter (0.8): low-passing the reference made the streaks worse (the
  line to line phase jumps are real).

CVBS decoder 0.8
----------------

### New
- Ref. filter (Off, 1/2, 1/4, 1/8): the colour reference measured on the
  burst of each line is low-passed over lines. Less line to line
  saturation / hue noise (horizontal streaks on saturated colours); with
  a jittery source clock, colour lines can come back at the stronger
  settings. Test option, default Off.

CVBS decoder 0.7.1
------------------

### Reverted
- The 0.7 colour reference changes (48-sample burst gate, colour gain
  low-passed over lines): no visible effect on the hardware, the lines on
  saturated colours come from something else. Logic freed (the FPGA was
  at 100%). Burst level (B) back to the 0.6 scale.

CVBS decoder 0.7
----------------

### Fixed
- Edge color sup. caused dark bands at the edges between colours with very
  different luma (e.g. white -> blue), which no Chroma delay value could
  fix. It is now always off with S-Video (no cross-colour there).
- Faint horizontal lines on saturated colours (e.g. cyan): the colour
  reference of each line came from a 32-sample burst measurement, whose
  noise changed the saturation / hue line by line. The burst is measured
  over 48 samples (same centre, Burst position unchanged), and the colour
  gain follows the burst amplitude low-passed over lines; the phase is still
  taken from each line (TVP7002 clock jitter).
- Diagnostics: the burst level (B) reads ~1.5x higher (longer gate).

### Removed
- Chroma delay (0.6): the colour offsets it was meant for came from Edge
  color sup., and the default alignment is already right; the logic was
  needed for the changes above (the FPGA is full).

CVBS decoder 0.6
----------------

### New
- Sharpness (Off, 1-7): luma peaking, sharper pixel edges. 7-tap filter
  with no gain at DC and at the Nyquist frequency (no noise boost).
- Chroma delay (-4..+4 samples, ~37ns each): aligns the colour with the
  luma edges.
- Settings of 0.3-0.5 are kept (migrated).

CVBS decoder 0.5.1
------------------

### Fixed
- Output Line2x 31kHz: "OUT OF RANGE!" after half a second. The CPU checked
  the raw watchdog status, also while the watchdog is bypassed for Line2x.

CVBS decoder 0.5
----------------

### Fixed
- Flickering horizontal bands (light / dark, 5-10 lines tall, at fixed
  heights that moved with the colour system), less visible at higher ADC
  gain. The blank level was measured over the burst with a residual that
  beat with the burst phase, and its rounded value toggled between two codes
  inside the field. The measurement now cancels the burst (6-sample box
  filter, 30-sample mean), and the level is updated only at the start of a
  field, with hysteresis.

CVBS decoder 0.4
----------------

### New
- Test option "Output: Line2x 31kHz" for HDMI TVs: the decoder inputs are
  line doubled (240p / 480i -> 1440x480p, 288p / 576i -> 1440x576p, ~54MHz,
  pixels repeated 2x). Never kept: the output is 15kHz at every boot. The
  15kHz watchdog is off only while a Line2x mode is output.

CVBS decoder 0.3
----------------

### Fixed
- Color system Auto: switched between colour and B&W every second or so.
  The colour killer is briefly on after the vsync (no burst), and the CPU
  read the status right then; it is now sampled in the middle of the field.
- Picture too dark, Contrast without effect: the FPGA used a fixed luma gain
  (right for ADC gain 4) instead of the one computed by the CPU from the ADC
  gain and Contrast.

### New
- Narrower chroma band-pass (two stages): less luma demodulated as colour.
- "Edge color sup." (default Medium): chroma attenuated around sharp luma
  edges, against the colour fringes at black / white edges.
- "Clamp position": TVP7002 clamp start (for tests).

CVBS decoder 0.2
----------------

### Fixed
- Coloured horizontal lines: the chroma phase came from the burst of the
  previous lines, and the line-locked sampling clock jitters from line to
  line. Now every line is demodulated with the reference from its own burst
  (the active video is delayed by 64 samples so that it is ready in time).
- Blank level too low (ADC code ~19): the burst and the dark saturated
  colours were clipped at the bottom of the ADC range. The Y input now uses
  the mid-level clamp with a coarse offset (blank at ~97, menu "ADC offset",
  replaces "ADC CM offset"); default ADC gain 2. Settings of 0.1 are not
  loaded.

CVBS decoder 0.1
----------------

- First test version: composite / S-Video, PAL and NTSC.

v1.6.1
------

### Fixed
- 480p / 640x480 Wide: shimmer on horizontal motion. The ADC took 1392 /
  1456 samples per line, not an integer ratio of the source pixels. Now
  480p is sampled at the source pixel clock (as Full 16:9 / Center 4:3)
  and each stored pixel is the exact average of 3 source pixels (240
  pixels); 640x480 takes 1280 samples per line, 4 averaged (256 pixels,
  2.5 source pixels each, the pattern repeating every 5). Slightly softer,
  and the line buffer timing margin is about 3 times larger. Content that
  is itself scaled (e.g. 256 pixels shown on 512) can still shimmer a
  little: its pixels are not a multiple of the stored ones.

v1.6
----

### New
- "Format" (was "HD format") also for 480p, 640x480 and 576p, for
  anamorphic 16:9 sources: Center 4:3 shows the middle 3/4 of the width
  (540 / 480 pixels) at the full width, full resolution; Wide (480p and
  640x480) shows the whole picture letterboxed in 160 lines (3 input lines
  each, aspect ratio 2:1 instead of 16:9), 292 / 290 pixels (4 ADC samples
  averaged), with ~5ms of delay (as 720p Wide, the line buffer absorbs the
  rows written over the whole frame and read in 160 lines). 576p has no
  Wide (192 rows would need more line buffer rows than available): it shows
  Full. The 2nd button cycles the formats for these sources too.

### Changed
- 480i from 3 input lines per output line (720p Full 16:9 / Center 4:3,
  1024x768, 480p / 640x480 Wide) with "Line blending" On: both fields are
  now a weighted mix of 2 lines, field A = 3/4 of line 0 + 1/4 of line 1,
  field B = 1/4 of line 1 + 3/4 of line 2 (was line 0 alone / the average
  of lines 1-2), so the two fields are equally sharp. With "Line blending"
  Off, or the flicker filter on, unchanged.
- Output PLL: VCO at most 1250MHz (was 1300MHz, the datasheet maximum).
  Same output timings; only the PLL dividers change, with some Width
  values (480p / 640x480 Wide, 576p, 1080p Center 4:3).
- Full resolution 4:2:2 modes store an even number of pixels (640x480
  Wide: 290 instead of 291).

### Fixed
- 640x480 Wide 480i: cyclic disturbance (stable, then more and more torn
  lines at the top, then stable again): the output PLL ran at 1278MHz,
  too close to its limit.
- With "Line phase" > 0, the last row could use lines after the active
  picture that were not captured (720p, 480p, 640x480, 576p, 1280x960 and
  1080p Wide): up to 4 blank lines after the active ones are now captured.

v1.5
----

### New
- 1024x768 input (60Hz, VESA standard timing 1344x806): 240p and 480i,
  3 lines per output line, the 960x720 center window (32 pixels and 24
  lines cropped per side), full resolution.
- 1280x1024 input (60Hz, VESA standard timing 1688x1066): 240p and 480i,
  4 lines per output line, the 960 center lines, 2 samples averaged.
- "Flicker filter" also for HD and PC sources with 480i output: each field
  line averages more input lines (1080p Full / Center 4:3, 1280x1024,
  1280x960, 720p Wide: 3 lines at levels 1-2, 4 at levels 3-4, instead of
  2; 720p and 1024x768: 3 lines in field A, 2 in field B, one strength), the
  two fields staying half an output line apart. The last field B line may
  use the first lines after the active picture (blank: it gets a bit
  darker); with no lines left (720p "Line phase" 2, 720p Wide phases 2-3,
  1280x960 phases 1-3) the filter is off.
- 2nd button on the OSSC: short press = 240p / 480i, long press (1 second)
  = next HD format (Full 16:9 / Center 4:3 / Wide). It changed the scanline
  mode, not available in this firmware. Like the menu changes, not saved
  until "Save settings".

### Changed
- Output PLL: VCO at least 650MHz (was 600MHz, the datasheet minimum), for
  more margin. Same output timings; only the PLL multipliers change, with
  some Width values (480i from 1080p / 720p Center 4:3 and Wide, 1024x768,
  480p, 640x480; 576i from 576p; 240p from 480p).

v1.4
----

### New
- "HDMI range" menu option: Full (0-255), Limited (16-235), Limited
  (16-255), as the MiSTer's Direct Video options. The FPGA scales the RGB
  values sent to the HDMI transmitter (every output, menu and test pattern
  included); the AVI InfoFrame signals limited range for both limited
  choices. Settings saved with older versions start with Full.
- "Test pattern" menu option (last item): Gray (the stock pattern), Grid,
  Monoscope, RGB, for the 15kHz test pattern shown at start, with no sync or
  an unsupported input. Generated by the FPGA; not saved (Gray at start).
  - Grid: 16x12 square cells on a 4:3 screen, centre dots (geometry,
    convergence)
  - Monoscope: circles (round on a correctly adjusted 4:3 screen), color
    bars, sharpness gratings, gray steps, RGB blocks
  - RGB: red, green, blue and white ramps and steps


v1.3
----

### New
- 1280x960 input (60Hz, VESA standard timing 1800x1000): 240p and 480i,
  4 lines per output line (exactly 240 rows: ideal for 320x240 content
  scaled x4), 640 pixels.
- 1080p Wide is now 216p: 1080 -> 216 lines (5 per output line) with the
  whole width, 480 pixels (4 averaged), squeezed horizontally (aspect ratio
  not kept). More detail than the 180p mode it replaces; "HD format" has one
  Wide choice (720p: 180p, 1080p: 216p).
- H. / V. position also for 15kHz sources in passthrough (240p, 480i,
  480i -> 240p): the picture moves without being changed; past the porch
  limits the edges are cropped (moving down keeps the source's vertical
  front porch, so the display keeps its sync).
- Menu: "480i input" is now "15kHz input", "480i filter" is now "Flicker
  filter".

### Fixed
- 4-line blending was 1 low on odd values (rounding).

### Internal
- Line buffer RAM output registered (output clock timing margin).
- README: HDMI-to-VGA DAC compatibility note; 50Hz HD sources not supported.


v1.2
----

### New
- 1080p Wide (180p): the whole 16:9 picture letterboxed, 1848x1080 -> 308x180
  (6x6 pixel average; with line blending the 4 middle lines of each group of
  6), 240p and 480i output. "HD format" now offers Wide for 1080p too.
- 576p input (50Hz): 288p (310–314 lines, 15.5–15.7kHz) and 576i output,
  every source pixel sampled as for 480p, optional flicker filter in 576i.
- 576i -> 288p ("480i input" set to 240p) verified on hardware (PS2).

### Internal
- 6-sample horizontal box filter and groups of up to 6 input lines in the
  FPGA line buffer.


v1.1
----

### New
- 1080p input (60Hz), 240p and 480i output, 4 input lines per output line:
  - Full 16:9: the 1708x960 center window (~89% of the picture), squeezed to 4:3
  - Center 4:3: the 1280x960 center window
  - best with sources scaled x4 vertically (integer scaling)
- "480i input" menu option: 480i sources can be shown as 240p, with both
  fields on the same scanlines (no interlacing). Passthrough remains the
  default. PAL (576i -> 288p) is not tested yet.
- "480i filter" menu option (Off, 1–4): flicker filter for 480p and 640x480
  sources with 480i output. Each field line is mixed with the lines above and
  below (weights a, 1-2a, a; level 4 = 1/4, 1/2, 1/4).
- Component input (YPbPr) verified.

### Changed
- "720p format" renamed to "HD format" (applies to 720p and 1080p; Wide is
  720p only, 1080p uses Full 16:9).
- Output line rate limits narrowed to 14.8–16.0kHz; the hardware watchdog
  now trips at 16.4kHz (was 16.9kHz).
- The LCD shows `DS vX.Y 15kHz` at start.

### Fixed
- Wide (180p): the sampling phase was spread over the 4-sample group and
  sampled on the pixel edges, causing shimmer on motion.
- Line buffer timing: the read/write window is now checked at the first and
  the last row, needed when the input is written faster than it is read
  (1080p).

### Internal
- Capture logic pipelined for the 148.5MHz 1080p pixel clock (capture side
  constrained at 150MHz).


v1.0
----

First release.

- Inputs (VGA, ~60Hz): 1280x720p, 720x480p, 640x480p
- Outputs: 240p (260–264 lines, 15.6–15.9kHz) or 480i (525 lines), frame
  locked to the input
- 720p formats: Full 16:9 (squeezed), Center 4:3, Wide (180p letterbox)
- Full resolution sampling with an exact output clock ratio (smooth
  horizontal scrolling); 480p / 640x480 stored in full RGB
- Composite sync on HSYNC for SCART: Off (H+V), XOR, AND, Standard
  (equalizing pulses and serrated vsync)
- Line blending, line phase, adjustable width and position, manual 480p /
  640x480 input selection
- 15kHz-only output: computed modes, 15.73kHz test pattern fallback and an
  FPGA hardware watchdog
- Runs from the secondary firmware slot; own settings sector, independent of
  the official firmware and profiles
