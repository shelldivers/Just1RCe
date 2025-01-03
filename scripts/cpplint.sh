#!/bin/bash

# Initialize cpplint report
echo ' ' > "$1"

# Read file_list.txt and run cpplint
while IFS= read -r file; do
    "$2" "$file" >> "$1" 2>&1
    echo "" >> "$1"
done < "$3"
