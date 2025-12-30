#!/bin/sh
set -eu

CC=clang
SRC=main.c
OUT=main


$CC \
  -Wall -Wextra \
	-v \
  "$SRC" \
  -o "$OUT" \
  -lX11 \
	-lm	
echo

echo "== Binary info =="
file "$OUT"
ldd "$OUT"
echo

echo "== Running =="
LD_DEBUG=libs ./"$OUT"
