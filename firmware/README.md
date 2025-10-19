# Dual-IQ Ham Radio DSP Firmware and Hardware Core

## Overview
This project implements a modern dual-IQ receiver/transmitter platform for 2 m and 70 cm amateur bands.
It features:

- IQ baseband architecture using LTC5594 and TLV320AIC3204
- CTCSS, DCS, and DTMF tone encode/decode
- Simple C++ DSP scaffolding for embedded processing
- I2S HAL abstraction for portability

## Contents
- **dual_iq_block_diagram.svg** — System overview
- **Firmware** — C++ sources for DSP and audio handling
- **dual_iq_bom.csv** — Suggested hardware BOM
- **pinmap.json** — MCU pinout template

## License
Hardware under CERN-OHL-W; firmware under Apache 2.0.
