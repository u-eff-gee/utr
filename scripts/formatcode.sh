#!/bin/bash

# Format main() program and code in src/ and include/

if [[ "$PWD" != *"utr" ]]; then
  cd ..
  if [[ "$PWD" != *"utr" ]]; then
    echo "Current working directory was not recognized as the utr root directory or a subfolder of it! Aborting..."
    exit
  fi
fi

echo "Applying llvm style to source code…"
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, \
             UseTab: ForIndentation, TabWidth: 4}" -i \
             utr.cc src/*.cc include/*.hh

# Format detector constructions
for i in `ls DetectorConstruction/`; do
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, \
             UseTab: ForIndentation, TabWidth: 4}" -i \
DetectorConstruction/$i/*.hh DetectorConstruction/$i/*.cc;
done
