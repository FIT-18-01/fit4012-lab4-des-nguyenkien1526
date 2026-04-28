#!/usr/bin/env bash
# DES sample test: verify the known ciphertext for the official Q2 vector.
set -euo pipefail

EXPECTED="01111110101111110100010010010011001000111111101011111010111110000100000010010001101001000010011111010110110001100000111000110100"
PLAINTEXT="00010010001101000101011001111000100110101011110011011110111100011010101010101010"
KEY="0001001100110100010101110111100110011011101111001101111111110001"

if [[ ! -x ./des ]]; then
  g++ -std=c++17 -Wall -Wextra -pedantic des.cpp -o des
fi

OUTPUT=$(printf '1
%s
%s
' "$PLAINTEXT" "$KEY" | ./des)
ACTUAL=$(printf '%s
' "$OUTPUT" | grep -oE '[01]{64,}' | tail -n 1)

if [[ "$ACTUAL" != "$EXPECTED" ]]; then
  echo "[FAIL] DES sample mismatch"
  printf 'Expected: %s
' "$EXPECTED"
  printf 'Actual:   %s
' "$ACTUAL"
  exit 1
fi

echo "[PASS] DES sample vector matches expected ciphertext."
