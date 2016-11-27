//
//  main.cpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#include <stdio.h>
#include <cstring>

#include "DDSCompressor.hpp"
#include "DDSDecompressor.hpp"

int main(int argc, const char * argv[])
{
    if (argc <= 3)
    {
        printf("Usage: ddsconverter [-c | -e] sourcefile targetfile\n");
        printf("\n");
        printf("-c      Compress the specified source file to DDS format.\n");
        printf("        The file should be in BMP format.\n");
        printf("\n");
        printf("-e      Decompress the specified source file into BMP format.\n");
        printf("        Only non-alpha DXT1 format is supported.\n");
        return 0;
    }
    
    if (strncmp(argv[1], "-c", 2) == 0)
    {
        Bitmap* image = NULL;
        
        DDSCompressor compressor(image);
        compressor.compress(argv[3]);
    }
    else if (strncmp(argv[1], "-e", 2) == 0)
    {
        DDSDecompressor decompressor(argv[2]);
        Bitmap* image = decompressor.decompress();
        
    }
    
    return 0;
}
