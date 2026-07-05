# Debroforce (3DS Broforce Clone)

A lightweight, optimized "Broforce" clone designed for the Nintendo 3DS (specifically tested/optimized for Old 3DS).

## Features
- **3 Iconic Bros**: Rambro, Brominator, Blade.
- **Destructible Terrain**: Grid-based tilemap (brown/green blocks).
- **Dual Screen Support**: Top screen action, bottom screen mini-map.
- **Improved Codebase**: Encapsulated state, modular asset management.

## Asset Integration
Place all `.png` assets in the `gfx/` folder before compiling for PC.
For 3DS, the engine is prepared for `citro2d` texture loading.

## Building

### For PC (Linux/macOS)
Requires SDL2 and SDL2_image.
```bash
cd debroforce
make pc
./debroforce
```

### For 3DS
Requires `devkitPro`, `devkitARM`, `libctru`, and `citro2d`.
Double-click `compile_mac.command` on macOS or run:
```bash
make 3ds
```

## Controls
- **Circle Pad / D-Pad**: Move
- **A**: Attack (and Start Game in Menu)
- **B**: Special
- **Start**: Toggle Menu / Playing
