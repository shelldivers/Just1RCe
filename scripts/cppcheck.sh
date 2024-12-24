#!/bin/bash

find . \( -name "*.cc" -path "./srcs/**" \) -o \( -name "*.h" -path "./includes/**" \) > file_list.txt

if [ ! -s file_list.txt ]; then
	echo "No C++ files found."
	rm -rf file_list.txt
	exit 0
else
	xargs cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem --xml --output-file=report.xml < file_list.txt
	rm -rf file_list.txt
fi
