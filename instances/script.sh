#!/bin/bash

# Base URL
BASE_URL="https://people.brunel.ac.uk/~mastjjb/jeb/orlib/files"

# Loop from 41 to 44
for i in {a..c}
do
    # Construct the URL and output filename
    URL="${BASE_URL}/cap${i}.txt"
    OUTPUT_FILE="cap${i}.txt"

    # Download the file
    curl -o "${OUTPUT_FILE}" "${URL}"

    # Check if the download was successful
    if [ $? -eq 0 ]; then
        echo "Downloaded ${OUTPUT_FILE} successfully."
    else
        echo "Failed to download ${OUTPUT_FILE}."
    fi
done
