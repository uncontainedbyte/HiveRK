# Hive Research Kernel

> **Status:** Research experiment / reference implementation
> **Architecture:** x86 (32-bit)
> **Size:** ~45 KB total (core + modules), ~13 KB core
> **Dependencies:** None (no external libraries, filesystems, or toolchains beyond a basic cross-compiler)

---

## 1. What This Project Is

Hive is a **research-oriented, modular operating system core** whose sole responsibility is to **discover, load, and connect independently compiled system modules at runtime**.

The core acts as a *bootloader-sized kernel nucleus*. It does not implement drivers, memory management, interrupt handling, scheduling, filesystems, or user interfaces directly. Instead, *all functionality exists as modules* that are dynamically located and connected after boot.

Each module:

* Is compiled and linked **entirely on its own**
* Has **no static dependencies** on other modules
* Exposes functionality via an exported command/function table
* Interacts with other modules only through runtime-discovered call indirection

The result is a system where **module placement, ordering, and internal implementation are irrelevant** to the core.

---

## 2. What This Project Is NOT

Hive is **not**:

* A production-ready operating system
* A general-purpose desktop OS
* A monolithic kernel
* A POSIX-compatible environment
* A performance-optimized kernel

This project intentionally prioritizes **clarity, modularity, and experimentation** over completeness or conventional OS structure.

---

## 3. Design Philosophy

### 3.1 Zero-Policy Core

The core ("the Queen"):

* Boots the system
* Locates modules in memory
* Builds a global call/command table
* Connects modules to one another

It does **not**:

* Set up memory management
* Register interrupts
* Load drivers
* Implement scheduling
* Enforce policy

All system behavior emerges from the modules.

---

### 3.2 Fully Dynamic Modularity

* No static linking between modules
* No compile-time knowledge of addresses
* No fixed load order
* No dependency graph

Modules identify functionality symbolically (e.g. `MODULE_A : CMD_1`), and the core resolves calls at runtime.

---

### 3.3 Learning-First Architecture

Hive was designed (retroactively discovered) to **dramatically lower the barrier to OS development**.

New developers can:

1. Use the core as-is
2. Write a single module
3. Boot and see real results

Without needing to understand:

* Linker scripts
* Full kernel architecture
* Monolithic subsystem interactions

---

## 4. The Hive Metaphor

The system is modeled after a bee hive:

* **Queen (Core):**

  * Coordinates the system
  * Knows where everything is
  * Does almost no work

* **Bees (Modules):**

  * Perform all real tasks
  * Are specialized
  * Are replaceable
  * Cooperate via known signals

The hive survives through cooperation, not centralization.

---

## 5. System Overview

### 5.1 Boot Process

1. System boots into the core (which also functions as the bootloader)
2. Core scans memory for valid modules
3. Each module registers its exported commands
4. Core builds a global call table
5. Control is handed off to initial system modules

Modules can exist in **any order** after the core binary.

---

### 5.2 Binary Layout

```
[ CORE ][ MODULE ][ MODULE ][ MODULE ] ...
```

* No filesystem required
* No metadata blocks
* No fixed offsets
* Modules are discovered dynamically

---

## 6. Module Interface (ABI Overview)

Each module provides:

* A module header identifying it as valid
* An export table defining:

  * Command identifiers
  * Function entry points
* Optional initialization and teardown routines

The core:

* Validates the module
* Registers its exports
* Makes them callable by other modules via indirection

**Important:**

* Modules do not know where other modules are
* Modules do not call functions directly
* All inter-module calls go through the core-managed call table

---

## 7. Implemented Modules (Example)

The following subsystems were implemented as modules during development:

* Physical and virtual memory management
* Interrupt handling
* Basic terminal / text UI

These are reference implementations only and exist to validate the architecture.

---

## 8. Performance Characteristics

* Small runtime overhead due to call indirection
* Negligible memory overhead given system scale
* Overall performance is sufficient for research, experimentation, and learning

Performance was not the primary design goal.

---

## 9. Why Development Stopped

Hive achieved its original (unplanned) goal:

> **Prove that a fully dynamic, non-linked, module-driven OS architecture is feasible, simple, and usable.**

However, it became clear that:

* The project was a **kernel construction framework**, not a personal OS
* Further development would turn it into infrastructure rather than an opinionated system

At that point, development was intentionally stopped and the project was reclassified as a **research experiment**.

---

## 10. Intended Use Cases

* OS development education
* Systems research
* Kernel architecture experimentation
* Modular OS prototyping
* Reference implementation for dynamic kernel design

---

## 11. License

This project is released under a permissive license. Use it, modify it, fork it, or build something entirely different on top of it.

(See LICENSE file.)

---

## 12. Final Notes

This repository exists as:

* A proof of concept
* A learning tool
* A snapshot of a completed research idea

If you build something from it — great.
If you learn from it — even better.
