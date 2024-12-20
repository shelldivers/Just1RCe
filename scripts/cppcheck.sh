#!/bin/bash

find . \( -name "*.cc" -path "./srcs/**" \) -o \( -name "*.h" -path "./includes/**" \) > file_list.txt

if [ ! -s file_list.txt ]; then
	echo "No C++ files found."
	rm -rf file_list.txt
	exit 0
else
	xargs cppcheck --language=c++ --enable=all --suppress=missingInclude --xml --output-file=not_filtered_report.xml < file_list.txt
	rm -rf file_list.txt

	xmllint --xpath '//error[@severity="warning" or @severity="error"]' not_filtered_report.xml > cppcheck_report.xml
	rm -rf not_filtered_report.xml
fi
