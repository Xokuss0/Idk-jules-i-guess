# Learnings: 3DS/PC Cross-Platform Development

## HAL Pattern
Using a Hardware Abstraction Layer (HAL) was critical for this project.
- `hal.h` defines the interface (drawing, input, system state).
- `hal_3ds.c` implements it using `libctru` and `citro2d`.
- `hal_sdl.c` implements it using `SDL2`.

This allowed for development and testing in a standard Linux environment where `devkitPro` installation was restricted, while maintaining a clear path to a 3DS binary.

## Optimization for Old 3DS
- **Tilemap**: Using a simple array and O(1) lookups for collisions keeps the CPU load low.
- **Particles**: Strictly limiting particle count and using simple rects instead of complex sprites for debris.
- **Mini-map**: Rendering the mini-map as a 1:1 pixel representation of the tilemap is extremely efficient.
