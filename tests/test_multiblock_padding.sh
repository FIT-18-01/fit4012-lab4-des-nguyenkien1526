#!/usr/bin/env bash
# Multi-block and zero padding test: plaintext longer than 64 bits must be padded correctly.
set -euo pipefail

PLAINTEXT="00010010001101000101011001111000100110101011110011011110111100011010101010101010"
KEY="0001001100110100010101110111100110011011101111001101111111110001"
EXPECTED="01111110101111110100010010010011001000111111101011111010111110000100000010010001101001000010011111010110110001100000111000110100"

if [[ ! -x ./des ]]; then
  g++ -std=c++17 -Wall -Wextra -pedantic des.cpp -o des
fi

CIPHER=$(printf '1
%s
%s
' "$PLAINTEXT" "$KEY" | ./des | grep -oE '[01]{64,}' | tail -n 1)

if [[ ${#CIPHER} -ne 128 ]]; then
  echo "[FAIL] Cipher length is not 128 bits"
  printf 'Actual length: %s
' "${#CIPHER}"
  exit 1
fi

if [[ "$CIPHER" != "$EXPECTED" ]]; then
  echo "[FAIL] Multi-block padding result mismatch"
  printf 'Expected: %s
' "$EXPECTED"
  printf 'Actual:   %s
' "$CIPHER"
  exit 1
fi

echo "[PASS] Multi-block zero padding works as expected."
