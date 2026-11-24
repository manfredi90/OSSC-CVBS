# CVBS decoder: ideas for later

Not planned for a release; notes to pick up again.

## Anti-jailbar filter (NES / SNES only)

- Likely cause: the 21.477MHz master clock of NTSC NES / SNES (6x the
  subcarrier) leaks into the video; sampled at 27MHz it folds to 5.52MHz
  (21.28MHz PAL SNES: 5.72MHz), fixed on screen (locked to the line). It
  beats with the 5.37MHz pixels and the TV scaler: vertical bars. Worse with
  the TVP7002 Video LPF off.
- Check first: SFC in RGB with / without bars (with bars: the console itself,
  the filter would only partly help).
- Filter: luma notch at 5.52MHz after the colour notch, optional (enable bit:
  sl_config2[7:6] are free).
  - 5-tap FIR (zeros at 5.52MHz and at Nyquist): ~60-80 LEs, 2-3 multipliers,
    about -3.5dB at 2.7MHz (finest SNES detail, Sharpness compensates).
  - narrow IIR biquad (~1MHz): keeps the pixel detail, ~100-130 LEs, slight
    ringing; may need logic freed elsewhere (diagnostics).
- Logic: 97% combinational in 0.38 (0.38 first version did not fit with
  ~160 LEs more).

## Black level readout (`K` in Decoder diag., 0.37 only)

- Minimum of 16-sample luma block averages in the active area of the field,
  ADC codes above the blank level (about -2 of measuring bias in the
  simulation). Removed in 0.38 for logic space.
- Japanese Super Famicom, composite: K+6 (black ~6-7 IRE above blank, as an
  NTSC setup; brightness -6 matched RGB). NTSC 7.5 IRE option On / Auto fits.

## VHS TBC (phase 2)

- Separate document (VHS TBC phase 2).
