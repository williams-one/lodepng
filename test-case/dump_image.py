#!/usr/bin/env python3
import sys

def convert_file_to_header(file_path):
    # Extract filename without extension for the array name
    filename = file_path.split('/')[-1].split('.')[0]

    # Read binary data from file
    with open(file_path, 'rb') as f:
        binary_data = f.read()

    # Create header file content
    header_content = f"""#pragma once

#include <stdint.h>

const uint8_t {filename}_map[] = {{
"""

    # Format bytes into lines of 16 values
    for i in range(0, len(binary_data), 16):
        line = binary_data[i:i+16]
        formatted_line = ', '.join(f'0x{b:02x}' for b in line)
        header_content += f"    {formatted_line}"
        if i + 16 < len(binary_data):
            header_content += ","
        header_content += "\n"

    header_content += "};\n"

    # Write to .h file
    output_file = f"{filename}.h"
    with open(output_file, 'w') as f:
        f.write(header_content)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python dump_image.py <image_path>")
        sys.exit(1)

    file_path = sys.argv[1]
    convert_file_to_header(file_path)