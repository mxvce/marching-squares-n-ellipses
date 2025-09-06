#!/bin/bash

cmake -S . -B build
cmake --build build --parallel --target sdl-min
