#!/bin/bash
# Navigate to the script's directory
cd "$(dirname "$0")"

echo "=== Setting up devkitPro Environment ==="
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM

echo "=== Cleaning and Compiling for 3DS ==="
make clean
make 3ds

if [ $? -eq 0 ]; then
    echo "========================================="
    echo " SUCCESS: The game compiled perfectly!   "
    echo "========================================="
else
    echo "========================================="
    echo " ERROR: Compilation failed.              "
    echo "========================================="
fi

# Keeps the window open so I can read any potential errors
read -p "Press Enter to exit..."
