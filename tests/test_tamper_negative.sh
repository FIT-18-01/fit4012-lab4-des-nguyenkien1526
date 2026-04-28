#!/usr/bin/env bash
# Tamper negative test: flipping 1 bit in ciphertext must not decrypt back to the original plaintext.
set -euo pipefail

PLAINTEXT="0001001000110100010101100111100010011010101111001101111011110001"
KEY="0001001100110100010101110111100110011011101111001101111111110001"

if [[ ! -x ./des ]]; then
  g++ -std=c++17 -Wall -Wextra -pedantic des.cpp -o des
fi

CIPHER=$(printf '1
%s
%s
' "$PLAINTEXT" "$KEY" | ./des | grep -oE '[01]{64,}' | tail -n 1)
TAMPERED="${CIPHER:0:10}$([[ ${CIPHER:10:1} == 0 ]] && printf 1 || printf 0)${CIPHER:11}"
DECRYPTED=$(printf '2
%s
%s
' "$TAMPERED" "$KEY" | ./des | grep -oE '[01]{64,}' | tail -n 1)

if [[ "$DECRYPTED" == "$PLAINTEXT" ]]; then
  echo "[FAIL] Tamper test failed: decrypted plaintext still matches original"
  exit 1
fi

echo "[PASS] Tamper negative test passed."
