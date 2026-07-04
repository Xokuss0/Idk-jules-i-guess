# Debroforce (3DS Broforce Clone)

A lightweight, optimized "Broforce" clone designed for the Nintendo 3DS (specifically tested/optimized for Old 3DS).

## Features
- **3 Iconic Bros**:
  - **Rambro**: Assault rifle and grenades.
  - **Brominator**: Minigun with knockback and invincibility special.
  - **Blade**: Melee attacks, dash, and throwing knives.
- **Destructible Terrain**: Grid-based tilemap where explosions and bullets destroy the environment.
- **Dual Screen Support**:
  - **Top Screen**: Action-packed gameplay.
  - **Bottom Screen**: Real-time mini-map and status display.
- **Cross-Platform**: Can be built for PC (SDL2) for rapid testing or 3DS (libctru/citro2d).

## Building

### For PC (Linux/macOS)
Requires SDL2.
```bash
cd debroforce
make pc
./debroforce
```

### For 3DS
Requires `devkitPro`, `devkitARM`, `libctru`, and `citro2d`.
```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM
# (Requires makerom and bannertool for .cia)
make 3ds
```

## Controls
- **D-Pad/Circle Pad**: Move
- **A**: Attack
- **B**: Special
- **L/R**: Dash (Blade)
- **Start**: Exit
- **Select**: Swap Bro (Debug)
