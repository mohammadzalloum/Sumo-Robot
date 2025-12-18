# SUMO Robot Controller (Power & Control Unit)

![494876971-89bf1ff8-a2c9-4657-9f19-c809cdc8a300](https://github.com/user-attachments/assets/5e8692a4-bd68-43f9-b0a0-55a354024e41)

High-current, competition-grade control and power distribution unit for autonomous SUMO robots—built for reliability, modularity, and electrical safety.

## Overview
This repository documents a compact, industrial-style controller enclosure that integrates:
- Power input protection and distribution
- Motor driving (high current)
- Regulated logic power
- Control electronics interfaces (sensors / UI / comms)

The design focuses on clear separation between power and logic, thermal robustness, and fast serviceability during competitions.

## Key Features
- High-current battery input with dedicated **main fuse**
- External **emergency kill switch** that cuts **entire system power**
- Segregated rails: **motor power** (high current) and **logic power** (regulated / low noise)
- Secure wiring via screw terminals and service-friendly layout
- Enclosure designed for inspection and quick maintenance

## System Architecture
Battery Pack
│
├─ Main Fuse + Emergency Kill Switch
│
├─ Power Distribution Bus
│ ├─ Motor Drivers (High Current)
│ └─ DC-DC Regulators (Logic Power)
│
└─ Control Unit (MCU / Logic Board)
├─ Sensors (Line / Distance / IMU)
├─ User Interface (Buttons / LEDs)
└─ Communication Ports

## Hardware Modules
### Power & Protection
- Main fuse for short-circuit protection
- Kill switch accessible from the exterior
- Power rails separated to reduce noise and improve reliability

### Motor Control
- High-current motor drivers
- Low-loss routing and connectors
- Designed for high-torque SUMO motors

### Control & I/O
- MCU/control board integration
- Defined pinout for motors, sensors, UI, and expansion headers

### Thermal & Mechanical
- Passive heatsinking for power electronics
- Ventilated enclosure and component spacing for airflow
- Competition-ready enclosure with external access to key controls/indicators

## Pin Mapping & Data Files
Pin assignments and wiring references:
- `pins_connection.csv` — MCU pin mapping and signal routing
- `components.csv` — components list / reference data

> Always verify the latest pin mapping before powering the system.

## Safety Notes
- Place the **fuse before any active electronics**
- Ensure the kill switch disconnects **full system power** (not logic-only)
- Avoid exposed conductive parts and verify insulation/clearances
- Use proper wire gauge for peak motor current and secure strain relief

## Testing & Bring-Up (Recommended)
1. Continuity + short-circuit checks (unpowered)
2. Power-up logic rail only (motors disconnected)
3. Single-motor testing under load
4. Full-system stress testing in competition conditions

## Repository Layout
├── hardware/
│ ├── enclosure/
│ ├── schematics/
│ └── pcb/
├── docs/
│ ├── wiring_diagrams/
│ └── images/
├── pins_connection.csv
├── components.csv
└── README.md


## Roadmap
- Current sensing and telemetry
- Battery monitoring
- Modular motor driver boards
- EMI improvements (shielding / filtering)
- CAN / RS-485 support
