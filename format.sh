#!/bin/bash

FILES="./src/main.c
"

for f in $FILES; do
  indent $f -brf -linux -l10000
	rm $f~
done

