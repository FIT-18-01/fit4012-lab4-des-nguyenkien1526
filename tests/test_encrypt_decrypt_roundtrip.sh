#!/usr/bin/env bash
# Round-trip test: encrypt then decrypt must restore the original 64-bit plaintext.
set -euo pipefail

PLAINTEXT="0001001000110100010101100111100010011010101111001101111011110001"
KEY="0001001100110100010101110111100110011011101111001101111111110001"

if [[ ! -x ./des ]]; then
  g++ -std=c++17 -Wall -Wextra -pedantic des.cpp -o des
fi

CIPHER=$(printf '1\n%s\n%s\n' "$PLAINTEXT" "$KEY" | ./des | grep -oE '[01]{64,}' | tail -n 1)
PLAIN=$(printf '2\n%s\n%s\n' "$CIPHER" "$KEY" | ./des | grep -oE '[01]{64,}' | tail -n 1)

if [[ "$PLAIN" != "$PLAINTEXT" ]]; then
  echo "[FAIL] Round-trip failed"
  printf 'Expected: %s\n' "$PLAINTEXT"
  printf 'Actual:   %s\n' "$PLAIN"
  exit 1
fi

echo "[PASS] DES round-trip restore plaintext successfully."
