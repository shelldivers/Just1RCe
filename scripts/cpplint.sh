#!/bin/bash

source ~/goinfre/venv/bin/activate

echo '' > cpplint_report.txt

find . \( -name "*.cc" -path "./srcs/**" \) -o \( -name "*.h" -path "./includes/**" \) > file_list.txt

if [ ! -s file_list.txt ]; then
	echo "No C++ files found."
	rm -rf file_list.txt
	exit 0
else
	while IFS= read -r file; do
    	cpplint "$file" >> cpplint_report.txt 2>&1
		echo "" >> cpplint_report.txt
	done < file_list.txt

	rm -rf file_list.txt
fi
