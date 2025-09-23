#!/usr/bin/env bash

set -e # Fail on command error

BIN="$1"

if [[ -z $BIN || ! -x $BIN ]]; then
  echo "Error: $BIN not found or not executable."
  echo "Compile it first with:"
  echo "    make or make all"
  exit 1
fi

for file in tests/*.lam; do
  echo "==== Running on $file ===="
  echo
  ./$BIN "$file" -l
  ./$BIN "$file" -p
  echo
done
