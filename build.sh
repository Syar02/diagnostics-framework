#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

echo "[healthd] Configuring..."
cmake -B "$BUILD_DIR" -S "$PROJECT_DIR" -DCMAKE_BUILD_TYPE=Release

echo "[healthd] Building..."
cmake --build "$BUILD_DIR" --parallel "$(nproc)"

echo "[healthd] Done → $BUILD_DIR/healthd"