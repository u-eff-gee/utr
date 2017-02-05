#!/bin/bash

echo "Applying llvm style to source code…"
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, \
             UseTab: ForIndentation, TabWidth: 4}" -i \
             utr.cc src/*.cc include/*.hh
