#!/bin/bash

FILES="
./src/audio.c
./src/audio.h
./src/const.h
./src/i2s.c
./src/i2s.h
./src/log.c
./src/log.h
./src/lut.c
./src/main.c
./src/math.c
./src/midi.c
./src/picox.h
./src/qfmt.h
"

for f in $FILES; do
  indent $f -brf -linux -l10000
	rm $f~
done


