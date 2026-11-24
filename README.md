OSSC CVBS decoder firmware
==========================

An unofficial secondary firmware for the [Open Source Scan Converter](https://github.com/marqs85/ossc)
that decodes **composite video and S-Video (PAL and NTSC)** in the FPGA and
outputs **15kHz RGB on HDMI** (240p / 288p / 480i / 576i, unchanged timing),
for RGB monitors and TVs through an HDMI→VGA DAC and a VGA→SCART or BNC cable.

It runs from the OSSC's secondary firmware slot ("Launch 2nd FW") and never
touches the official firmware: power cycling the OSSC always boots the
official firmware again.

> **Unofficial, experimental and AI.** This project was made with the help of AI. The code was written together with Claude. It's an unofficial fork of the original OSSC firmware.
> Use at your own risk. I am not responsible for any damage to the OSSC or the monitor

> **Status: test version (0.14).**


Inputs
------

| Input | Connection |
|-------|------------|
| AV2 Composite | composite video on the AV2 **Y** (green) jack |
| AV2 S-Video   | S-Video **Y** on the AV2 Y (green) jack, **C** on the AV2 **Pb** (blue) jack |

Remote control: button 5 selects AV2 Composite, button 2 AV2 S-Video. The
other inputs keep working as 15kHz RGB passthrough; 31kHz and HD sources show
the test pattern.


How it works
------------

* The TVP7002 samples the line-locked input at ~27MHz (1716 samples per line
  for 525 lines, 1728 for 625), with no colour space conversion, the blank
  level clamped at ADC code ~97 (mid-level clamp + coarse offset) and a gain
  that leave room for the sync, the burst and the chroma below blank and
  above white, and the clamp after the burst.
* The FPGA decoder: chroma band-pass, quadrature demodulation with a DDS
  (increment computed from the measured line rate and the subcarrier
  frequency), low-pass filters, phase and automatic chroma gain from the
  burst of the same line (the line-locked sampling clock jitters from line to
  line; PAL switch from the burst alternation), a
  frequency loop for the source / OSSC crystal tolerances, colour killer,
  luma with an adaptive notch at the subcarrier, YUV → RGB.
* Colour system **Auto**: 3.58 or 4.43MHz from the colour killer, PAL or NTSC
  from the burst alternation (525 lines: NTSC, PAL-M, PAL-60, NTSC 4.43; 625
  lines: PAL, PAL-N). SECAM is not supported.


CVBS decoder menu
-----------------

| Item | Values | |
|------|--------|---|
| Output | 15kHz, Line2x 1440, Line2x 720, Line3x 47kHz, Line4x 63kHz | 15kHz: as the source (CRT). Line2x 1440: 1440x480p / 1440x576p at 54MHz (every sample), sent as 480p / 576p with pixel repetition. Line2x 720: 720x480p / 720x576p at 27MHz, exactly CEA-861 (one sample out of 2; with the crystal clock the output clock is the crystal itself): try it if an HDMI TV drops a frame now and then in Line2x 1440. Line3x / Line4x: 720 wide, 786 / 1048 lines (NTSC) at 40.5 / 54MHz, not CEA modes (as the stock Line3x / Line4x). Saved with the settings; the 15kHz watchdog is off while a line multiplied mode is output: do not use those with 15kHz monitors (back to 15kHz with the remote) |
| Color system | Auto, PAL, NTSC, PAL-60, NTSC 4.43, PAL-M, PAL-N | |
| Saturation | 0–200% | |
| Hue | ±30 deg | |
| Contrast | 50–160% | |
| Brightness | ±31 | |
| Sharpness | Off, 1-7 | luma peaking: sharper pixel edges (the luma of a composite signal is softened by the notch). No boost at DC or at the highest frequencies (no extra noise). Default Off |
| Luma notch | On, Off | Off: full bandwidth luma (dot crawl visible) |
| Edge color sup. | Off, Low, Medium, High | chroma attenuated around sharp luma edges: less colour fringing (cross-colour) at black / white edges, slightly softer colour edges, dark bands at edges between colours of very different brightness at high settings. Always off with S-Video. Default Medium |
| CSync on HSYNC, HDMI range, H. / V. position | | as in the OSSC Downscaler firmware (CSync: Off (H+V, default, HDMI TVs), XOR, AND, Standard (equalizing pulses + serrated vsync, for picky CRTs)) |

CVBS advanced menu
------------------

| Item | Values | |
|------|--------|---|
| ADC gain | 0–15 | TVP7002 coarse gain (full scale shown in Vpp), default 2 |
| ADC offset | ±31 | blank level = ADC code 128 + offset (default -31: ~97, shown as `L` in the diagnostics) |
| Burst position | 120–220 | start of the burst gate, samples after the hsync leading edge (default 165) |
| Clamp position | 0–120 | TVP7002 clamp start, samples after the hsync trailing edge (default 92: after the burst) |
| Sample clock | Line-locked, 27MHz xtal | ADC clock: the OSSC 27MHz crystal (default: steady colour phase, lines re-timed on their sync, output timing from the crystal), or the TVP7002 H-PLL locked to the hsync (the colour phase drifts along the line) |
| Line re-timing | On, Off | 27MHz xtal only: each line is re-timed on its sync trailing edge (fractional delay of the luma), so the picture does not shake. Off only for tests. Default On |
| Ref. filter | Auto, Off, On | colour reference averaged over the lines (less line to line colour noise). Auto: On with the xtal clock, Off line-locked. Default Auto |
| Adv. timing | | as in the OSSC firmware |
| Diagnostics > Decoder diag. | | `Bn` burst level, `Ln` blank level (ADC code), `Pn` highest ADC code in the picture (255: white clipped, lower the ADC gain), then `K` colour killer / `P` PAL burst alternation / `N` none (NTSC) |
| Diagnostics > Sync diag. | | per second: `O` output frames, `F` input frames, `S` output resyncs (xtal clock: one per input frame) |
| Diagnostics > Xtal timing | | `L` line period (27MHz clocks), `O` output line length (`O..>..`: being redone) |
| Diagnostics > Event count. | | since power on: `P` setups, `U` register updates, `X` output resyncs out of place |

The LCD shows the mode and the colour system on the first row and the
diagnostics on the second.

Settings are saved with **Settings opt. → Save settings** (stored in this
firmware's own sector, in a format of its own).


Simulation
----------

`sim/cvbs/cvbs_sim.py` generates PAL / NTSC / S-Video colour bars (with
subcarrier and line rate errors, noise, vertical blanking) and runs the
decoder RTL with Icarus Verilog.


Installation
------------

1. Copy `ossc_cv_vX.Y.bin` to a micro SD card.
2. In the **official** firmware, install it with the firmware update function
   of the settings menu. The image carries the secondary-firmware key, so it
   is written to the secondary slot only.
3. Start it with **Launch 2nd FW**.

To return to the official firmware, power cycle the OSSC.


Building
--------

The firmware is built by GitHub Actions (`.github/workflows/build.yml`) with
Quartus Lite and the RISC-V toolchain (four fitter seeds in parallel, the best
timing is kept); the output is `ossc_cv_vX.Y.bin`, with the version taken
from `DS_VERSION` in `software/sys_controller/inc/firmware.h`.


Credits and license
-------------------

* Based on the [Open Source Scan Converter](https://github.com/marqs85/ossc)
  by Markus Hiienkari and contributors (see [CREDITS](CREDITS)), and on the
  OSSC Downscaler firmware.
* License: GPLv3, same as the OSSC project (see [LICENSE](LICENSE)).
