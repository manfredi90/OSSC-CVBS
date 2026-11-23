Changelog
=========

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
