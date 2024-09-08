#!/bin/bash

FILES="
app0/main.c
app1/main.c
"

for f in $FILES; do
  indent $f -brf -linux -l10000
	rm $f~
done

