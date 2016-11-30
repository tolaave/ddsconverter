//
//  DDSCompressor.cpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#include <stdio.h>
#include <algorithm>
#include <array>

#include "DDSFormat.h"

#include "DDSCompressor.hpp"

DDSCompressor::DDSCompressor(Bitmap* bitmap) :
    m_bitmap(bitmap),
    m_quantizer()
{
}

int DDSCompressor::compress(const char *targetfile)
{
    FILE* fp = fopen(targetfile, "wb");
    if (!fp)
    {
        printf("Could not create target file %s\n", targetfile);
        return -1;
    }
    
    DDSHeader header;
    header.dwMagic = 0x20534444; //'DDS '
    header.dwSize = 124;
    header.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    header.dwWidth = m_bitmap->getWidth();
    header.dwHeight = m_bitmap->getHeight();
    header.dwPitchOrLinearSize = std::max<uint32_t>(1, ((header.dwWidth + 3) / 4)) * 8;
    header.dwDepth = 0;
    header.dwMipMapCount = 0;
    ::memset(header.dwReserved, 0, sizeof(header.dwReserved));
    header.ddspf.dwSize = 32;
    header.ddspf.dwFlags = DDPF_FOURCC;
    header.ddspf.dwFourCC = 0x31545844;
    header.ddspf.dwRGBBitCount = 0;
    header.ddspf.dwRBitMask = header.ddspf.dwGBitMask = header.ddspf.dwBBitMask = header.ddspf.dwABitMask = 0;
    header.dwCaps = header.dwCaps2 = header.dwCaps3 = header.dwCaps4 = header.dwReserved2 = 0;
    
    if (fwrite(&header, 1, sizeof(header), fp) != sizeof(header))
    {
        printf("Could not write header to file %s\n", targetfile);
        fclose(fp);
        return -2;
    }
    
    int x = 0;
    int y = 0;
    while (y < header.dwHeight)
    {
        compressBlock(x, y, fp);
        x += BLOCK_WIDTH;
        if (x >= m_bitmap->getWidth())
        {
            x = 0;
            y += BLOCK_HEIGHT;
        }
    }
    return 0;
}

void DDSCompressor::compressBlock(int x, int y, FILE* fp)
{
    std::vector<ARGBPixel> sourceColors(BLOCK_SIZE, 0);
    for (int y = 0; y < BLOCK_HEIGHT; y++)
        for (int x = 0; x < BLOCK_WIDTH; x++)
        {
            sourceColors[y * BLOCK_WIDTH + x] = m_bitmap->getPixel(x, y);
        }
    
    std::array<ARGBPixel, 2> quantizedColors;
    m_quantizer.quantize(sourceColors, quantizedColors);
}
