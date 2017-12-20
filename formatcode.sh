#!/bin/bash

# Format main() program and code in src/ and include/
echo "Applying llvm style to source code…"
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, \
             UseTab: ForIndentation, TabWidth: 4}" -i \
             utr.cc src/*.cc include/*.hh 

# Format detector constructions
for i in `ls DetectorConstruction/`;
do
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, \
             UseTab: ForIndentation, TabWidth: 4}" -i \
DetectorConstruction/$i/*.hh DetectorConstruction/$i/*.cc;
done;
