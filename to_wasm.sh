#!/usr/bin/env bash
set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

NAME="${1:?Usage: $0 <filename_without_extension>}"
SRC="examples/${NAME}.c"
OUT_DIR="examples/to_wasm"
OUT="examples/to_wasm/${NAME}.wasm"
CC="${CC:-clang}"

if [ ! -f "$SRC" ]; then
    echo -e "${RED}Error: Source file not found: $SRC${NC}"
    exit 1
fi

if ! command -v "$CC" &> /dev/null; then
    echo -e "${RED}Error: Compiler '$CC' not found in PATH.${NC}"
    exit 1
fi

echo "Using: $(which "$CC")"

mkdir -p "$OUT_DIR"

FLAGS=(
    "--target=wasm32"
    "-O3"
    "-flto"
    "-nostdlib"
    "-Wl,--no-entry"
    "-Wl,--export=render"
    "-Wl,--export=pixels"
    "-Wl,--import-memory"
    "-Wl,--allow-undefined"
    "-Iinclude"
)

"$CC" "${FLAGS[@]}" -o "$OUT" "$SRC" "src/dsgl.c"

echo -e "${GREEN}Built: $OUT${NC}"
