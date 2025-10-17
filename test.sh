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
  echo "File Content:"
  cat $file

  echo
  echo "Lex:"
  ./$BIN "$file" -l

  echo
  echo "Parse:"
  ./$BIN "$file" -p

  echo
  echo "Interpret:"
  ./$BIN "$file"

  echo
done
