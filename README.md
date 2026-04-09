# Marine Power Controller Monorepo

This repository contains the two STM32 firmware targets that make up the marine power control system.

## Layout

- `platformio.ini`: repo-level PlatformIO configuration
- `boards/marine_stm32f070c6tx.json`: custom PlatformIO board definition for the STM32F070C6T6 target
- `src/controller`, `include/controller`: active controller firmware sources and headers
- `src/client`, `include/client`: active node firmware sources and headers
- `include/shared`: protocol types and framing shared by both targets
- `config/stm32/stm32f0xx_hal_conf.h`: shared HAL configuration header
- `ldscripts/STM32F070C6Tx_FLASH.ld`: shared linker script
- `legacy/cubemx/controller`: archived STM32CubeMX export from `marine_power_controller_master_firmware`
- `legacy/cubemx/client`: archived STM32CubeMX export from `marine_power_controller_8out_firmware`
- `docs/controller-code-flow.md`: controller runtime walkthrough
- `docs/client-code-flow.md`: node runtime walkthrough

## Build

Install PlatformIO Core if it is not already available:

```sh
python3 -m pip install --user platformio
```

Build both firmware targets:

```sh
pio run
```

Build one target:

```sh
pio run -e controller
pio run -e 8-outputs-1
pio run -e 8-outputs-2
pio run -e 8-outputs-3
pio run -e 6-inputs-1
pio run -e 6-inputs-2
pio run -e 6-inputs-3
```

The root `Makefile` provides the same build entrypoints:

```sh
make
make controller
make 8-outputs-1
make 8-outputs-2
make 8-outputs-3
make 6-inputs-1
make 6-inputs-2
make 6-inputs-3
make clean
```

## Flash And Debug

Upload with ST-Link:

```sh
pio run -e controller -t upload
pio run -e 8-outputs-1 -t upload
pio run -e 8-outputs-2 -t upload
pio run -e 8-outputs-3 -t upload
pio run -e 6-inputs-1 -t upload
pio run -e 6-inputs-2 -t upload
pio run -e 6-inputs-3 -t upload
```

Start a debug session:

```sh
pio debug -e controller
pio debug -e 8-outputs-1
```

PlatformIO uses the repo-local custom board definition in `boards/marine_stm32f070c6tx.json` and the shared linker script in `ldscripts/STM32F070C6Tx_FLASH.ld`.

Node firmware environments:

- `8-outputs-1`: 8-output card, `NODE_ID=10`
- `8-outputs-2`: 8-output card, `NODE_ID=11`
- `8-outputs-3`: 8-output card, `NODE_ID=12`
- `6-inputs-1`: input board, `NODE_ID=1`
- `6-inputs-2`: input board, `NODE_ID=2`
- `6-inputs-3`: input board, `NODE_ID=3`

## Documentation

Code flow documents:

- `docs/controller-code-flow.md`
- `docs/client-code-flow.md`

## History

The commit history from the original repositories was imported into this monorepo under subdirectories, so the old project history is preserved. The original STM32CubeMX exports remain archived under `legacy/cubemx/`.

Original repository mappings:

- `projects/controller` <= `git@github.com:minigubben/marine_power_controller_master_firmware.git`
- `projects/client` <= `git@github.com:minigubben/marine_power_controller_8out_firmware.git`
