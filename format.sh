#!/bin/bash

FILES="
app0/main.c
app1/main.c
app3/main.c
app4/main.c
common/util.c
common/util.h
common/log.c
common/log.h
"

for f in $FILES; do
  indent $f -brf -linux -l10000
	rm $f~
done

