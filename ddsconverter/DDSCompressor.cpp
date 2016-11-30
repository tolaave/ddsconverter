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
#include <stdlib.h>

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
        DDSBlockData block = compressBlock(x, y, fp);
        
        if (fwrite(&block, 1, sizeof(block), fp) != sizeof(block))
        {
            printf("Could not write block %d,%d to %s\n", x, y, targetfile);
            fclose(fp);
            return -3;
        }
        
        x += BLOCK_WIDTH;
        if (x >= m_bitmap->getWidth())
        {
            x = 0;
            y += BLOCK_HEIGHT;
        }
    }
    return 0;
}

DDSBlockData DDSCompressor::compressBlock(int blockX, int blockY, FILE* fp)
{
    std::vector<Color32> sourceColors(BLOCK_SIZE, 0);
    for (int y = 0; y < BLOCK_HEIGHT; y++)
        for (int x = 0; x < BLOCK_WIDTH; x++)
        {
            sourceColors[y * BLOCK_WIDTH + x] = m_bitmap->getPixel(x + blockX, y + blockY);
        }
    
    std::pair<Color32,Color32> quantizedColors;
    m_quantizer.quantize(sourceColors, quantizedColors);
    
    std::array<Color32, 4> finalColors =
    {
        C32_ROUND16(quantizedColors.first),
        C32_ROUND16(quantizedColors.second),
        C32_ROUND16(INTERPOLATE_C32(quantizedColors.first, quantizedColors.second, 2, 1)),
        C32_ROUND16(INTERPOLATE_C32(quantizedColors.first, quantizedColors.second, 1, 2))
    };

    DDSBlockData result;
    
    Color16 c0 = C32_TO_16(finalColors[0]);
    Color16 c1 = C32_TO_16(finalColors[1]);
    if (c0 < c1)
    {
        SWAP(finalColors[0], finalColors[1]);
        SWAP(c0, c1);
        SWAP(finalColors[2], finalColors[3]);
    }
    else if (c0 == c1)
    {
        result.wColor0 = c0;
        result.wColor1 = 0xffff;
        result.dwData = 0x00000000;
        return result;
    }
    
    result.dwData = 0;
    
    for (int y = 0; y < BLOCK_HEIGHT; y++)
        for (int x = 0; x < BLOCK_WIDTH; x++)
        {
            Color32 srcColor = C32_ROUND16(m_bitmap->getPixel(x + blockX, y + blockY));
            int bits = chooseClosestIndex(srcColor, finalColors);
            result.dwData |= ((bits << (x * 2)) << (y * 8));
        }
    
    result.wColor0 = c0;
    result.wColor1 = c1;
    
    return result;
}

int DDSCompressor::chooseClosestIndex(Color32 color, std::array<Color32, 4>& colors)
{
    float minSquaredDistance = (float)UINT32_MAX;
    int minIndex = 0;
    
    int index = 0;
    for (auto it = colors.begin(); it != colors.end(); it++)
    {
        Color32 c = *it;
        int rDist = (int)GET_R32(c) - (int)GET_R32(color);
        int gDist = (int)GET_G32(c) - (int)GET_G32(color);
        int bDist = (int)GET_B32(c) - (int)GET_B32(color);
        
        float squaredDistance = (rDist * rDist * LUMINANCE_R) + (gDist * gDist * LUMINANCE_G) + (bDist * bDist * LUMINANCE_B);
        if (squaredDistance < minSquaredDistance)
        {
            minSquaredDistance = squaredDistance;
            minIndex = index;
        }
        index++;
    }
    
    return minIndex;
}
