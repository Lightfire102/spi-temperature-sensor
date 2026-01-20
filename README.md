# SPI Temperature Sensor (TMP121)

## Overview
In this project I used a digital temperature sensor using the SPI-based TMP121, focusing on low-level protocol understanding, firmware, and hardware debugging using common industry tools such as the Sigilent oscilloscope and Salae logic analyzer.

The goal of this project is not just to read temperature data in my room, but to build a deeper understanding of how SPI communication works in regards to signal and timing.

## Project Goals
- Create SPI communication from nothing (using Arduino Uno R3)
- Read and decode temperature data from TMP121
- Verify SPI timing and data using lab instruments
- Document design decisions, debugging methods, and results
- Produce a full technical walkthrough (YouTube video + repo)

## Hardware (Planned)
- Arduino Uno R3
- TMP121 SPI Temperature Sensor
- Hardware components (capacitor, SMD to DIP adapter, etc.)
- Breadboard & jumper wires

## Tools
- Oscilloscope
- Logic Analyzer
- Serial Monitor
- Serial Plotter

## Repository Structure
```text
spi-temperature-sensor/
├── schematics-wiring/    # Circuit schematic and wiring diagrams
├── firmware/             # SPI driver and application code
├── oscilloscope/         # Scope captures and timing analysis
├── logic-analyzer/       # SPI decode captures
└── notes/                # Datasheet notes and debugging insights
