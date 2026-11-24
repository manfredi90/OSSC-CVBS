OSSC CVBS Decoder firmware
==========================

An unofficial secondary firmware for the [Open Source Scan Converter](https://github.com/marqs85/ossc)
that turns the OSSC into a **composite video and S-Video decoder (PAL and
NTSC)**: the colour decoding is done in the FPGA, and the picture comes out in
**RGB on HDMI**, either at the source's own 15kHz timing (240p / 288p / 480i /
576i) for RGB CRTs and 15kHz monitors (through an HDMI→VGA DAC and a
VGA→SCART or BNC cable), or line doubled / tripled / quadrupled for HDMI TVs.
No frame buffer, no added frame of lag.

It is a sibling of the [OSSC Downscaler](https://github.com/manfredi90/OSSC-Downscaler)
(HD → 15kHz) and of the [OSSC Downscaler HD](https://github.com/manfredi90/OSSC-Downscaler-HD)
(HD → 1080i / 540p). All of them run from the OSSC's secondary firmware slot
("Launch 2nd FW"), so only one can be installed at a time; none touches the
official firmware: power cycling the OSSC always boots the official firmware
again.


> **Unofficial, experimental and AI.** This project was made with the help of AI. The code was written together with Claude. It's an unofficial fork of the original OSSC firmware.
> Use at your own risk. I am not responsible for any damage to the OSSC or the monitor


Thanks
------

This project was only possible because the OSSC project is an incredible project and has confirmed its versatility over the years. Thanks to Markus Hiienkari, all the credit goes to him.


Safety
------

* **15kHz output by default.** With **Output** on 15kHz, three independent
  layers keep the output at 15kHz:
  1. the output timing is the source's own (15kHz in, 15kHz out);
  2. no sync or an unsupported input shows an internal 15kHz test pattern;
  3. a hardware watchdog in the FPGA, independent of the CPU, blanks video and
     all syncs if the output line rate leaves the 15kHz range.
* **Line2x / Line3x / Line4x are for HDMI TVs and multisync monitors only.**
  The watchdog is off while one of them is selected, and the choice is saved
  with the settings: after saving it, the OSSC starts at 31 / 47 / 63kHz (test
  pattern included). **Do not connect a 15kHz monitor then**: set Output back
  to 15kHz first (the OSSC's LCD shows the menu, so it can be done with the
  remote control without a picture).
* **The official firmware is never modified.** The image only fits the
  secondary slot (flash 0x80000–0xEFFFF). This firmware's settings live in a
  single sector of its own (0xF0000) and it never reads or writes the official
  profiles, its SD card profiles or the primary image.


Requirements
------------

* OSSC board (tested on rev 1.8) with the official firmware (tested with 1.22)
  as the primary firmware
* Composite or S-Video source on the **AV2** RCA jacks:

  | Input | Connection |
  |-------|------------|
  | AV2 Composite | composite video on the AV2 **Y** (green) jack |
  | AV2 S-Video   | S-Video **Y** on the AV2 Y (green) jack, **C** on the AV2 **Pb** (blue) jack (S-Video to 2 RCA adapter) |

* Display:
  * **15kHz:** OSSC HDMI output → HDMI-to-VGA DAC → VGA-to-SCART (or VGA/BNC)
    cable → 15kHz RGB display. Composite sync for SCART can be sent on the
    HSYNC line ("CSync on HSYNC"). As with the OSSC Downscaler, not every DAC
    handles 15kHz signals: DACs that work with the MiSTer FPGA's Direct Video
    mode also work here.
  * **HDMI TVs:** Output on Line2x (720x480p / 720x576p, a standard HDMI
    format), Line3x or Line4x, CSync Off (H+V).


Supported inputs and outputs
----------------------------

| Input | Colour systems | Timing |
|-------|----------------|--------|
| AV2 Composite, AV2 S-Video | **Auto** (default), PAL, NTSC, PAL-60, NTSC 4.43, PAL-M, PAL-N | 240p, 288p, 480i, 576i (standard and non-standard timings, e.g. NES and SNES) |

| Output | Lines (NTSC / PAL) | Pixel clock | Notes |
|--------|--------------------|-------------|-------|
| 15kHz | as the source | 27MHz | for CRTs and 15kHz monitors |
| Line2x 31kHz | 525 / 625 (480p / 576p) | 27MHz | 720x480p / 720x576p exactly as CEA-861 (HDMI VIC 2/3, 17/18) |
| Line3x 47kHz | 786 / 936 | 40.5MHz | not a CEA mode (as the official firmware's Line3x) |
| Line4x 63kHz | 1048 / 1248 | 54MHz | not a CEA mode; the sharpest in the tests on an HDMI TV |

* The TVP7002 samples the input with the OSSC's 27MHz crystal (default) or
  line-locked (H-PLL). With the crystal clock the colour subcarrier phase is
  steady along the line, every line is re-timed on its sync with 1/16 sample
  precision, and the output timing follows the source frame by frame.
* Colour decoding in the FPGA: chroma band-pass, quadrature demodulation with
  a DDS locked to the source subcarrier (frequency loop), burst phase and
  automatic chroma gain from the bursts of each line (interpolated along the
  line), PAL switch from the burst alternation, colour killer.
* Colour system **Auto**: 3.58 or 4.43MHz from the colour killer, PAL or NTSC
  from the burst alternation (525 lines: NTSC, PAL-M, PAL-60, NTSC 4.43; 625
  lines: PAL, PAL-N). SECAM is not supported.
* Luma / chroma separation (composite): an **adaptive notch** at the
  subcarrier. NTSC: the colour estimate is compared with the line above (2
  lines) or the two lines above (3 lines, chosen automatically for NES / SNES,
  whose subcarrier turns 120 degrees per line): fine luma detail stays sharp,
  the colour is removed. S-Video: no separation needed, full bandwidth luma.
* The test pattern and the NO SYNC screen are shown at the selected Output.


Installation
------------

1. Copy `ossc_cv_vX.Y.bin` to a micro SD card.
2. In the **official** firmware, install it with the firmware update function
   of the settings menu. The image carries the secondary-firmware key, so it
   is written to the secondary slot only (it replaces the OSSC Downscaler or
   the OSSC Downscaler HD if one of them was installed).
3. Start it with **Launch 2nd FW**. The LCD shows `CV vX.Y 15kHz` and the
   OSSC outputs the test pattern until a source is found.
4. Press button 5 on the remote control for **AV2 Composite** or button 2 for
   **AV2 S-Video**.

Buttons on the OSSC: the 1st one switches between AV2 Composite and AV2
S-Video; the 2nd one has no function in this firmware. The other inputs (AV1,
AV3) are not available. **Initial input** (Settings opt.): AV2 Composite
(default), AV2 S-Video or Last used.

To return to the official firmware, power cycle the OSSC.


Menu
----

### CVBS decoder

| Item | Values | |
|------|--------|---|
| Output | 15kHz, Line2x 31kHz, Line3x 47kHz, Line4x 63kHz | see the outputs table and **Safety**. Saved with the settings |
| Color system | Auto, PAL, NTSC, PAL-60, NTSC 4.43, PAL-M, PAL-N | Auto (default) recognizes the system by itself; force it only for a source that is misdetected |
| Saturation | 0–200% | |
| Hue | ±30 deg | |
| Contrast | 50–160% | |
| NTSC 7.5 IRE | Auto, Off, On | NTSC "setup": black 7.5 IRE above the blank level (NTSC-M, PAL-M). On: removed, so that black is black again (gain x100/92.5, white stays white). **Auto** (default): on with NTSC, NTSC 4.43 and PAL-M. The system cannot tell sources without setup: if dark greys look crushed, set Off |
| Brightness | ±31 | |
| Sharpness | Off, 1–7 | luma peaking: sharper pixel edges. No boost at DC or at the highest frequencies (no extra noise). Default Off |
| Luma notch | Adaptive, Off, Simple | **Adaptive** (default): sharp outlines and fine pixels, no grey halo beside them (NTSC; PAL as Simple). In **Decoder diag.** a `3` shows that the 3-line mode is in use. **Simple**: plain notch. **Off**: full bandwidth luma, the colour subcarrier shows as dot crawl |
| Edge color sup. | Off, Low, Medium, High | chroma attenuated around sharp luma edges: less colour fringing at black / white edges, softer colour edges and dark bands between colours of very different brightness at high settings. Always off with S-Video. Default Low |
| CSync on HSYNC | Off (H+V), XOR, AND, Standard | Off (default) for HDMI TVs; composite sync types are for RGBS monitors through an HDMI→VGA DAC; Standard has equalizing pulses and serrated vsync |
| HDMI range | Full (0-255), Lim. (16-235), Lim. (16-255) | RGB levels sent on HDMI. Full is right for most TVs and DACs; use Limited if blacks look grey or whites are clipped |
| H. position | ±40 | moves the picture horizontally |
| V. position | ±12 | moves the picture vertically |

### CVBS advanced

Technical settings: the defaults are right for most sources.

| Item | Values | |
|------|--------|---|
| ADC gain | 0–15 | TVP7002 coarse gain (full scale shown in Vpp), default 2 |
| ADC offset | ±31 | blank level = ADC code 128 + offset (default -31: ~97, shown as `L` in the diagnostics) |
| Burst position | 120–220 | start of the burst gate, samples after the hsync leading edge (default 165) |
| Clamp position | 0–120 | TVP7002 clamp start, samples after the hsync trailing edge (default 92: after the burst) |
| Sample clock | Line-locked, 27MHz xtal | 27MHz xtal (default): steady colour phase, lines re-timed on their sync, output timing from the crystal. Line-locked: the TVP7002 H-PLL locked to the hsync (the colour phase drifts along the line) |
| Line re-timing | On, Off | 27MHz xtal only: each line re-timed on its sync, so the picture does not shake. Off only for tests. Default On |
| Ref. filter | Auto, Off, On | colour reference averaged over the lines (less line to line colour noise). Auto (default): On with the xtal clock, Off line-locked |
| Adv. timing | | as in the official firmware |
| Diagnostics > Decoder diag. | | `Bn` burst level, `Ln` blank level (ADC code), then `K` colour killer / `P` PAL burst alternation / `N` none (NTSC), `3`: 3-line luma notch |
| Diagnostics > Sync diag. | | per second: `O` output frames, `F` input frames, `S` output resyncs (xtal clock: one per input frame) |
| Diagnostics > Xtal timing | | `L` line period (27MHz clocks), `O` output line length (`O..>..`: being redone) |
| Diagnostics > Event count. | | since power on: `P` setups, `U` register updates, `X` output resyncs out of place |

The LCD shows the mode and the colour system on the first row and the
diagnostics on the second.

Settings are saved with **Settings opt. → Save settings** (stored in this
firmware's own sector, independent of the official profiles). The OSSC
Downscaler and the OSSC Downscaler HD use the same sector with different
formats: settings are not carried over when switching between them, each
starts from its defaults.


Known issues and limitations
----------------------------

* Composite and S-Video only, on AV2; no RGB passthrough on the other inputs.
* SECAM is not supported.
* PAL: no adaptive luma notch (the V component alternates from line to line):
  the plain notch is used.
* No time base correction: unstable sources such as VHS tapes are shown as
  they are.
* Some HDMI TVs show a black frame now and then with 480p / 576p (Line2x); the
  same happens with the official firmware's Line2x on those TVs. Line3x or
  Line4x may help, as may **Sync opt. → FPGA PLL BW: Low**.
* NES and SNES may show faint vertical bars (jailbars), from the consoles'
  own clock; keep **Video in opt. → Video LPF** on its lowest setting.


Building
--------

The firmware is built by GitHub Actions (`.github/workflows/build.yml`) with
Quartus Lite and the RISC-V toolchain (four fitter seeds in parallel, the best
timing is kept); the output is `ossc_cv_vX.Y.bin`, with the version taken
from `DS_VERSION` in `software/sys_controller/inc/firmware.h`. See
[README_OSSC.md](README_OSSC.md) for the original OSSC build instructions.

`sim/cvbs/cvbs_sim.py` generates PAL / NTSC / S-Video test signals (colour
bars, stripes, subcarrier and line rate errors, noise, vertical blanking) and
runs the decoder RTL with Icarus Verilog.


Credits and license
-------------------

* Based on the [Open Source Scan Converter](https://github.com/marqs85/ossc)
  by Markus Hiienkari and contributors (see [CREDITS](CREDITS)), and on the
  OSSC Downscaler firmware.
* License: GPLv3, same as the OSSC project (see [LICENSE](LICENSE)).
