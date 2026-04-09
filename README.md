# Marine Power Controller Monorepo

This repository combines the two STM32 firmware projects that make up the marine power control system.

## Layout

- `projects/controller`: controller firmware, imported from `marine_power_controller_master_firmware`
- `projects/client`: client firmware, imported from `marine_power_controller_8out_firmware`
- `docs/controller-code-flow.md`: controller runtime walkthrough
- `docs/client-code-flow.md`: client runtime walkthrough

Each project keeps its existing `Makefile`, `CMakeLists.txt`, STM32CubeMX files, and local `.vscode` settings.

## Build

Build both projects from the repository root:

```sh
make
```

Build one project:

```sh
make controller
make client
```

Clean build artifacts:

```sh
make clean
```

You can also continue working inside each project directory directly:

- `projects/controller`
- `projects/client`

## Documentation

Code flow documents:

- `docs/controller-code-flow.md`
- `docs/client-code-flow.md`

## History

The commit history from the original repositories was imported into this monorepo under subdirectories, so the old project history is preserved.

Original repository mappings:

- `projects/controller` <= `git@github.com:minigubben/marine_power_controller_master_firmware.git`
- `projects/client` <= `git@github.com:minigubben/marine_power_controller_8out_firmware.git`
