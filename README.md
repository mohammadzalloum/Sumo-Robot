# 🥋 SUMO Robot Controller — Industrial Power & Control Unit

<p align="center">
  <img src="assets/controller-overview.jpg" alt="SUMO Robot Controller CAD Overview" width="600"/>
</p>
![494876971-89bf1ff8-a2c9-4657-9f19-c809cdc8a300](https://github.com/user-attachments/assets/cedd8313-ff62-4cfe-bd0c-31f1bcff627b)

> **High‑current, competition‑grade control and power distribution unit designed for autonomous SUMO robots.**
> Built with reliability, modularity, and electrical safety in mind.

---

## 📌 Project Overview

This project presents a **compact industrial-style controller enclosure** for a SUMO robot. It integrates **power management, motor driving, control electronics, and safety interfaces** into a single rigid unit suitable for harsh competition environments.

The design emphasizes:

* High current handling
* Thermal management
* Clear separation between power and logic
* Easy serviceability during competitions

The repository documents **hardware design, pin mapping, component selection, and integration guidelines**.

---

## 🎯 Key Objectives

* Provide a **robust electrical backbone** for a SUMO robot
* Minimize wiring complexity and failure points
* Support **high‑torque DC motors** with stable power delivery
* Ensure **competition safety compliance** (kill switch, fusing, insulation)
* Enable rapid debugging and maintenance

---

## 🧱 System Architecture

```
Battery Pack
   │
   ├── Main Fuse & Emergency Kill Switch
   │
   ├── Power Distribution Bus
   │      ├── Motor Drivers (High Current)
   │      └── DC‑DC Regulators (Logic Power)
   │
   └── Control Unit (MCU / Logic Board)
          ├── Sensors (Line, Distance, IMU)
          ├── User Interface (Buttons / LEDs)
          └── Communication Ports
```

---

## ⚙️ Hardware Highlights

### 🔋 Power System

* High‑current battery input (competition‑grade)
* Dedicated **main fuse** for short‑circuit protection
* **Emergency stop / kill switch** accessible from the exterior
* Segregated power rails:

  * Motor power (high current)
  * Logic power (regulated, low noise)

### 🚗 Motor Control

* Multiple high‑current motor drivers
* Screw terminals for secure motor connections
* Designed for **high torque, low RPM SUMO motors**
* Optimized copper paths and connectors to reduce losses

### 🧠 Control Electronics

* Central microcontroller / control board
* Clear pin mapping for:

  * Motor control
  * Sensors
  * User inputs
* Expansion headers for future upgrades

### 🌡 Thermal Management

* Passive heat‑sinks on power electronics
* Ventilated enclosure design
* Component spacing optimized for airflow

---

## 🧩 Enclosure Design

* Rigid, competition‑ready enclosure
* Transparent body for **visual inspection** during matches
* Bottom mounting plate compatible with SUMO chassis standards
* External access to:

  * Power switch
  * Kill switch
  * Status indicators

---

## 🔌 Pin Mapping & Connections

Detailed pin mappings and wiring tables are provided in:

* `pins_connection.csv`
* `components.csv`

These files define:

* MCU pin assignments
* Motor driver connections
* Power input/output mapping
* Sensor and interface wiring

> ⚠️ **Always verify pin mappings before powering the system.**

---

## 🛡 Safety Considerations

* Mandatory fuse before any active electronics
* Kill switch cuts **entire system power**, not logic only
* No exposed conductive parts
* Proper wire gauges selected for peak motor current

---

## 🧪 Testing Strategy

* Continuity and short‑circuit testing before power‑up
* Logic‑only power test (no motors connected)
* Single‑motor load testing
* Full‑system stress test under competition conditions

---

## 📂 Repository Structure

```
├── hardware/
│   ├── enclosure/
│   ├── schematics/
│   └── pcb/
├── docs/
│   ├── wiring_diagrams/
│   └── images/
├── pins_connection.csv
├── components.csv
└── README.md
```

---

## 🚀 Future Improvements

* Current sensing & telemetry
* Smart battery monitoring
* Modular motor driver boards
* EMI shielding enhancements
* CAN / RS‑485 communication support

---

## 🏆 Use Case

Designed specifically for:

* **Autonomous SUMO robot competitions**
* High‑impact, high‑current robotics applications
* Educational and research robotics platforms

---

## 📜 License

This project is released for **educational and competition use**.
See `LICENSE` for details.

---

## ✍️ Author

Developed by **Team Safeguard / SUMO Robot Project**
Focused on robust robotics, embedded systems, and competition‑grade design.

---

> 💡 *If you find this project useful, consider starring ⭐ the repository and contributing improvements.*
