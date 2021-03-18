#!/bin/bash

FILES="
./src/audio.c
./src/audio.h
./src/const.h
./src/ggm.h
./src/ggm_math.c
./src/i2s.c
./src/i2s.h
./src/log.c
./src/log.h
./src/main.c
./src/midi.c
./src/qfmt.h
"

for f in $FILES; do
  indent $f -brf -linux -l10000
	rm $f~
done
