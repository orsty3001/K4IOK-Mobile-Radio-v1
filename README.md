# VHF/UHF 2 Meter 70 CM 50 Watt Mobile Radio Prototype

An open-source dual-receiver mobile radio for 2 m and 70 cm amateur bands.
Features include:

- Dual receive (A/B) with independent S-meters and volume
- TX/RX CTCSS & DCS tone support
- VFO and channel ("MR") modes
- CHIRP-compatible serial interface
- Based on LTC5594 IQ demod, TLV320AIC3204 codec, and STM32H743 MCU
- Open KiCad hardware schematics and firmware sources

## Directory structure
- **hardware/** — KiCad schematics, PA/LPF data, dual-IQ front-end
- **firmware/** — Core code for DSP, UI, tone control, and VFO/channel ops
- **docs/** — Protocols, CSV schema, calibration procedures, and BOM

Released under **CERN‑OHL‑W** (hardware) and **Apache‑2.0** (software).

## Quick start
1. Open `hardware/dual_iq_top.kicad_pro` in KiCad 7+.
2. Build firmware from `firmware/fw/` using your ARM toolchain.
3. Load `channels.csv` for example memory channels.
4. Connect via serial 115200 bps and use commands in `serial_protocol.txt`.

## Architecture at a Glance

     Exciter (0–10 dBm)
            |
        20 dB Pad (drive limiter, flatness)
            |
      RF Band Switch (small relays or PIN pair)
        /                         \
  VHF drive path             UHF drive path
  (144–148 MHz)              (430–450 MHz)
   0.5–1 W driver              0.5–1 W driver
   6–10 dB pad                 6–10 dB pad
        |                         |
  RA60H1317M1A               RA60H4047M1
   Final @ 12.5 V             Final @ 12.5 V
        |                         |
  2 m LPF (50 Ω)             70 cm LPF (50 Ω)
        \                         /
           Post-PA Diplexer/Combiner (2-way)
                        |
                   Antenna port
                        |
               SWR coupler (dual) → MCU
                        |
                 T/R (PIN or relay)

## Control & Protection (common to both bands)

Goals: no splatter, no cooked modules, no angry finals after a bad antenna day.

Sequencing (PTT in → out):

T/R to TX → 2) Bias enable (soft-start VGG) → 3) Unmute drive.
Reverse for RX. Typical delays: 3–5 ms / stage (FM), 8–10 ms (SSB/digital).

Biasing:
Per-band VGG rails trimmed to your linearity target (quiescent current set per datasheet). Use temperature-aware bias (thermistor on module flange) to hold idle current constant with heat.

Fold-back:
Dual directional couplers (one per band) → rectify with detector diodes → op-amps to MCU ADC. If Reflected > threshold or Case Temp > limit, MCU reduces VGG (soft fold-back); if severe, gate drive mute and assert FAULT.

Power:
Each module can draw ~10–12 A at 50–60 W out (mode-dependent). Use a low-IR drop 13.8 V rail, 10–22 mΩ sense for OCP, and ≥ 4700 µF bulk per PA card plus 0.1 µF + 1 µF ceramics at pins.

## Post-PA Filtering

Even with matched modules, proper LPFs are mandatory. Target harmonic rejection:

2 m path: ≥ 50 dBc @ 288–444 MHz (2nd–3rd)

70 cm path: ≥ 50 dBc @ ≥ 860 MHz (2nd)

Component tech: silver-mica or NP0/C0G ceramics, high-Q air-core coils (14–16 AWG tinned copper), 50 Ω terminations. Use 2 oz copper and wide traces; keep coil-to-coil coupling low (right-angle coil orientation, ≥ one coil dia spacing).

Starting LPF Values (well-behaved, conservative)

These are “get-on-air” values you can tweak with VNA/spectrum analyzer. Choose 7-pole Chebyshev (0.1 dB ripple) for sharper knee; 5-pole Butterworth if you prefer gentler group delay.

2 m LPF (144–148 MHz), 7-pole Chebyshev, fc ≈ 170 MHz (50 Ω):
(Series L and shunt C alternating, begins with L)

L1 ≈ 120 nH (8 turns, 10 mm dia, 10 mm length, 14 AWG)

C2 ≈ 120 pF (2× 60 pF NP0 in parallel)

L3 ≈ 200 nH

C4 ≈ 82 pF

L5 ≈ 200 nH

C6 ≈ 120 pF

L7 ≈ 120 nH
Expect ~0.2–0.3 dB passband loss; > 55 dB @ 300 MHz if layout is careful.

70 cm LPF (430–450 MHz), 7-pole Chebyshev, fc ≈ 550 MHz (50 Ω):

L1 ≈ 47 nH (5 turns, 6 mm dia, 6 mm length, 16–18 AWG)

C2 ≈ 39 pF

L3 ≈ 82 nH

C4 ≈ 27 pF

L5 ≈ 82 nH

C6 ≈ 39 pF

L7 ≈ 47 nH
Expect ~0.3–0.4 dB passband loss; > 50 dB @ 900 MHz with compact layout and shield can.

Tip: for UHF, if your analyzer shows peaking at ~700–800 MHz, add a tiny 2–4 pF trimmer in parallel with the center shunt cap and retune for a flatter stopband.

## Post-PA Diplexer (Combiner)

Combines the already-filtered VHF and UHF paths to a single antenna, with isolation ≥ 40 dB between ports.

Topology: Low-pass leg for VHF, high-pass leg for UHF, both 50 Ω. Aim for fc ≈ 260–300 MHz.

Simple 5-pole set (starting numbers):

Low-pass leg (to VHF port), 5-pole Butterworth, fc ~ 280 MHz:

L ≈ 82 nH, C ≈ 56 pF, L ≈ 150 nH, C ≈ 39 pF, L ≈ 150 nH

High-pass leg (to UHF port), 5-pole Butterworth, fc ~ 280 MHz:
(dual of above; series caps/shunt inductors)

C ≈ 27 pF, L ≈ 47 nH, C ≈ 39 pF, L ≈ 82 nH, C ≈ 56 pF

Practical alternative: use a commercial VHF/UHF diplexer rated ≥ 100 W (mobile class) and skip building this network; still keep your per-band LPFs.

## Pre-Driver & Drive Management

Per band pre-driver: a linear 0.5–1 W MMIC or small LDMOS stage. Add a 6–10 dB fixed pad before the RA60 input; the modules need only ~50 mW at full Pout.

Drive limiter: add a PIN-diode shunt clamp or RF power limiter ahead of the pad (protects finals against hot-switching or exciter spikes).

Band switch: Two miniature RF relays (latching) are simpler and quieter than PIN networks at this power/voltage; interlock in firmware.

## Thermal & Mechanical

Heatsink interface: lap the base, use quality TIM (e.g., silicone-free pad or thin grease). Keep case < 90 °C at worst-case duty (data mode in summer cabin).

Airflow: front-to-rear blower with ducting past both modules and LPFs. Place the thermistor right at the module tab.

## Bias/Protect Controller (MCU-centric)

MCU: STM32G0/G4 or ATSAMD21 (12-bit+ ADCs, timers, low-latency).
Temperature: 10 k NTC epoxy bead glued to module flange.
Couplers: -30 to -40 dB stripline or ferrite RF couplers per band.
Detectors: ADL5513/ADL5519 (log) for simple calibration, or Schottky diode+op-amp (LT6236).
VGG generation:

DAC (MCP4725 I²C) → op-amp buffer → MOSFET-OR’d enable → to module VGG pin.

Add RC soft-start (e.g., 2–5 ms) and bleeder (100 kΩ) for quick bias collapse on FAULT.

Gate/Drive mute: RF relay de-energize or JFET shunt at pre-driver output.
OCP: 20 mΩ shunt + INA219/INA226 to MCU; trip ~18–20 A/module.
T/R control: high-side 12 V MOSFETs for relay coils with flyback TVS.
User I/O: PTT in, BAND select in (or auto), SWR bargraph out (LEDs), FAULT latch, TEMP.
Firmware logic (outline):

IDLE → PTT? → assert T/R → wait 5 ms → ramp VGG to setpoint → unmute drive.

Sample Forward/Reflected @ 1–5 kHz; compute SWR; fold-back table (e.g., SWR>2.5 → −3 dB; >3.0 → −6 dB; >4.0 → hard mute).

Temp fold-back above 80 °C; hard FAULT at 90 °C.

Debounce band-change; prevent hot band switching under RF.

## Bill of Materials (core)

Per-band PA card

Final module: RA60H1317M1A (2 m) / RA60H4047M1 (70 cm)

Bias: MCP4725 DAC, TLV9062/TLV9102 op-amps, AOZ1284 or LM2596 buck (VGG/VDD auxiliaries)

Coupler: etched or ferrite-core style; detector: ADL5513 or SMS7630 + LT6236

Pre-driver: 0.5–1 W MMIC/LDMOS (e.g., RFPA0133-class) + fixed pads

LPF parts: NP0/C0G & silver-mica caps, 14–18 AWG air-core coils, shield can

Combiner/diplexer & T/R

Two RF relays (latching, 50 Ω, >100 W hot-switch rating not required if sequenced)

Diplexer LC as above or commercial 100 W VHF/UHF diplexer

TVS across relay coils; MOSFET drivers (AOZ1282 or discrete)

Controller

MCU: STM32G0/ATSAMD21, 16–32 MHz

INA226 current monitor, NTC 10 k, MOSFETs for enables, 2× small OLED/LEDs (optional)

Power: 13.8 V in → 5 V/3.3 V bucks (≥3 A margin), TVS (SMBJ15A), common-mode choke

## Layout & Build Notes

Keep each band’s PA, LPF, and coupler on its own “strip,” with a tight RF ground and via fences; meet the diplexer at a single star point.

Short, wide bias returns to the module tab; decouple VDD/VGG at the pins (0.1 µF+1 µF+bulk).

Coil placement: rotate adjacent inductors 90° to reduce magnetic coupling; keep at least one coil diameter spacing; if space is tight, use shield walls between sections.

Shield cans over LPFs (especially UHF) can buy you 10–15 dB extra stopband stability in a cramped mobile chassis.

Serviceability: bring out test pads for VGG, Idq, FWD/REF voltages, and add a “TUNE” mode in firmware that keys at 5–10 W with manual bias adjust.



