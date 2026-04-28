#!/usr/bin/env bash
# Wrong key negative test: decrypting with an incorrect key must not recover the original plaintext.
set -euo pipefail

PLAINTEXT="0001001000110100010101100111100010011010101111001101111011110001"
KEY="0001001100110100010101110111100110011011101111001101111111110001"
WRONG_KEY="1111000011110000111100001111000011110000111100001111000011110000"

if [[ ! -x ./des ]]; then
  g++ -std=c++17 -Wall -Wextra -pedantic des.cpp -o des
fi

CIPHER=$(printf '1
%s
%s
' "$PLAINTEXT" "$KEY" | ./des | grep -oE '[01]{64,}' | tail -n 1)
DECRYPTED=$(printf '2
%s
%s
' "$CIPHER" "$WRONG_KEY" | ./des | grep -oE '[01]{64,}' | tail -n 1)

if [[ "$DECRYPTED" == "$PLAINTEXT" ]]; then
  echo "[FAIL] Wrong-key test failed: incorrect key still recovered the plaintext"
  exit 1
fi

echo "[PASS] Wrong-key negative test passed."
