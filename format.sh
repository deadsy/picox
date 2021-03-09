#!/bin/bash

FILES="
./src/main.c
./src/log.c
./src/log.h
"

for f in $FILES; do
  indent $f -brf -linux -l10000
	rm $f~
done

