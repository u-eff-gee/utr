#!/bin/bash

clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, \
	        UseTab: ForIndentation, TabWidth: 4}" -i src/*.cc
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, \
	        UseTab: ForIndentation, TabWidth: 4}" -i include/*.hh

