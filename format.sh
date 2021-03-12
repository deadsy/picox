#!/bin/bash

FILES="
./src/main.c
./src/log.c
./src/lut.c
./src/math.c
./src/audio.c
./src/i2s.c
./src/lut.h
./src/log.h
./src/const.h
./src/qfmt.h
./src/audio.h
./src/i2s.h
"

for f in $FILES; do
  indent $f -brf -linux -l10000
	rm $f~
done

