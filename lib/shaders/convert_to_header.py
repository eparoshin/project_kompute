import sys
import struct
from pathlib import Path

infile = sys.argv[1]
outfile = sys.argv[2]

with open(infile, "rb") as input:
    bytes_data = input.read()

uint32_data = [struct.unpack('I', bytes_data[i:i+4])[0] for i in range(0, len(bytes_data), 4)]

with open(outfile, "w") as output:
    output.write("#pragma once\n")
    output.write("#include <vector>\n")
    output.write("#include <cstdint>\n")
    output.write(f"namespace NS{Path(infile).name.split('.')[0]} {{\n")
    output.write("const std::vector<uint32_t> SpirvRawData = {\n")
    output.write(", ".join(map(hex, uint32_data)))
    output.write("};\n")
    output.write("}\n")
