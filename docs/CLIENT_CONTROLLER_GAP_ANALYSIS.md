# Client vs Controller Gap Analysis

Date: 2026-04-09

## Scope

This review compares the two active firmware targets in this repository:

- `src/client`
- `src/controller`

The focus is the functional contract between them, especially serial/RS485 communication and scene handling. Build integration is included only where it affects the current executable artifacts.

## Executive Summary

The PlatformIO migration fixed the earlier build-definition drift, but the two firmware targets are still not aligned enough to work as a complete system.

The biggest gap is still architectural: the client contains a draft receive-side protocol/parser, while the controller contains no corresponding transport, parser, command encoder, or scene engine. In practice, the client looks like an experimental node implementation and the controller still looks like a startup skeleton with a scene data structure stub.

## Current State

- The active firmware builds are now defined in `platformio.ini`.
- `pio run -e controller` builds the controller target and includes `src/controller/scenes.c`.
- `pio run -e client` builds the client target and includes `src/client/marine_controller.cpp`.
- The old STM32CubeMX exports are archived under `legacy/cubemx/` and are no longer the active build source of truth.

## Findings

### 1. No shared protocol implementation exists across both sides

The only protocol definition in the repo is a comment in the client firmware at `src/client/marine_controller.cpp`. It describes a frame format beginning with `0xAA`, then `length`, `checksum`, `command`, and payload.

The controller firmware has no equivalent parser, no frame builder, and no command dispatcher:

- `src/controller/main.c` only initializes hardware and then spins in an empty loop.
- `src/controller/scenes.c` only defines a struct.
- `include/controller/scenes.h` has comments only and exports no API.

Impact: there is no actual client/controller contract implemented on both ends.

### 2. The client parser is incomplete and currently not operational

The client starts UART reception in `src/client/marine_controller.cpp`, but the receive state machine is broken:

- `recived_counter` starts at `-1`.
- Start-byte detection only happens when `recived_counter < -1`.
- Since the reset value is exactly `-1`, the start byte `0xAA` is never accepted and the parser never enters frame collection.

Impact: even the client-side draft protocol receiver will not assemble frames as written.

### 3. Command semantics are documented, but not implemented

The client comment block defines commands for ACK/NAK, outputs, and scenes in `src/client/marine_controller.cpp`, but the command handler does not implement them.

Instead, `handleCommands()` only compares incoming data to `"test"` and toggles a GPIO.

Impact: the documented protocol and the executable behavior are different.

### 4. Scene models are incompatible and unshared

The client hardcodes scene data as a 3D byte array in `src/client/marine_controller.cpp`. It stores scene addresses and output IDs in a compact array with implicit meaning.

The controller defines a very different `struct Scenes` in `src/controller/scenes.c`, using four `int[50]` arrays:

- `in_address`
- `on_on_out_addresses`
- `on_off_out_addresses`
- `off_out_addresses`

There is no shared header, serialization format, or ownership model for scenes between the projects.

Impact: even if transport existed, the two sides do not agree on the shape of scene data.

### 5. The controller cannot receive UART interrupts in its current form

The client enables and handles USART2 interrupts:

- NVIC setup exists in `src/client/stm32f0xx_hal_msp.c`.
- `USART2_IRQHandler()` exists in `src/client/stm32f0xx_it.c`.

The controller does neither:

- No NVIC enable in `src/controller/stm32f0xx_hal_msp.c`.
- No `USART2_IRQHandler()` declaration in `include/controller/stm32f0xx_it.h`.
- No `USART2_IRQHandler()` implementation in `src/controller/stm32f0xx_it.c`.

Impact: the controller is not set up for interrupt-driven serial reception at all.

### 6. RS485 direction handling is not coordinated

Both projects define an RS485 direction pin, but they are different board mappings:

- Client: `include/client/main.h`
- Controller: `include/controller/main.h`

Both initialize the pin low at startup:

- Client: `src/client/main.c`
- Controller: `src/controller/main.c`

But neither side toggles RS485 TX enable around transmit operations, and the controller does not transmit at all.

Impact: there is no verified half-duplex bus ownership model.

### 7. Build integration is no longer the main blocker

The pre-migration repo had inconsistent Make/CMake source lists. That specific issue has been fixed by the PlatformIO migration:

- the client active build includes `src/client/marine_controller.cpp`
- the controller active build includes `src/controller/scenes.c`
- the old CubeMX-generated Make/CMake files are now archived under `legacy/cubemx/`

Impact: the current blockers are now protocol and runtime behavior, not source-list drift.

## Gap Matrix

| Area | Client | Controller | Gap |
|---|---|---|---|
| Transport setup | UART RX interrupt path present | UART initialized only | Controller cannot participate in same RX model |
| Framing | Draft frame format documented and partially parsed | No frame format implementation | No shared wire protocol |
| Command handling | Only test string GPIO toggle | No handlers | No shared command behavior |
| Scene engine | Hardcoded byte-array scenes | Unused struct definition | Different data models |
| RS485 bus control | Direction pin exists, no TX policy | Direction pin exists, no TX policy | No bus arbitration contract |
| Build integration | Active PlatformIO build includes custom logic | Active PlatformIO build includes stub logic | Build system no longer masks the runtime gaps |

## Recommended Order of Work

1. Define a shared protocol specification in one common document or header.
   Include frame format, checksum polynomial, endianness, command IDs, payload layout, and ACK/NACK behavior.

2. Decide role ownership clearly.
   The controller should be the command source and the client should be the actuator node, or vice versa, but this needs to be explicit.

3. Create a shared command/scene model.
   Avoid separate ad hoc representations in each project.

4. Fix transport plumbing first.
   Add UART IRQ support, RX state machine, and TX direction control on both sides before scene logic.

5. Only then implement scene execution.
   Otherwise scene work will be built on an undefined transport contract.

## Immediate High-Value Fixes

- Add one shared protocol header or markdown spec at repo root.
- Make the controller implement `USART2_IRQHandler`, UART RX startup, and a minimal frame parser.
- Fix the client RX state machine so it can actually detect the start byte.
- Decide whether `scenes` belong centrally in the controller or distributed to clients.

## Validation Notes

I inspected the active source tree directly and verified the current build entrypoints:

- `pio run -e controller`
- `pio run -e client`

I did not hardware-test upload or live debugging in this pass, so the runtime analysis above is based on source inspection plus successful local builds rather than on-target execution.
