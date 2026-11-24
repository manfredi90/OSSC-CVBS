Changelog
=========

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
