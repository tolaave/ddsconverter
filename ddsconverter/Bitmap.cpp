//
//  Bitmap.cpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#include <stdio.h>

#include "Bitmap.hpp"

#include "BMPFormat.h"

Bitmap::Bitmap (int width, int height) :
    m_width(width),
    m_height(height)
{
    m_data = new ARGBPixel[width * height];
}

void Bitmap::setPixel(int x, int y, ARGBPixel value)
{
    if (x >= 0 && y >= 0 && x < m_width && y < m_height)
    {
        m_data[y * m_height + m_width] = value;
    }
}

ARGBPixel Bitmap::getPixel (int x, int y)
{
    if (x >= 0 && y >= 0 && x < m_width && y < m_height)
    {
        return m_data[y * m_height + m_width];
    }
    return 0;
}

Bitmap* Bitmap::load (const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Could not open %s\n", filename);
        return nullptr;
    }
    
    BMPHeader header;
    if (fread(&header, 1, sizeof(BMPHeader), fp) != sizeof(BMPHeader))
    {
        printf("Error reading bitmap header of file %s\n", filename);
        fclose(fp);
        return nullptr;
    }
    
    if (header.wType != BM_TYPE)
    {
        printf("File %s is not BMP file\n", filename);
    }
    
    BitmapInfoHeader biHeader;
    if (fread(&biHeader, 1, sizeof(BitmapInfoHeader), fp) != sizeof(BitmapInfoHeader))
    {
        printf("Error reading bitmap info header of file %s\n", filename);
        fclose(fp);
        return nullptr;
    }
    
    if (biHeader.dwWidth >= 0x1fff || biHeader.dwHeight >= 0x1fff)
    {
        printf("Resolution of %s is out of range\n", filename);
        fclose(fp);
        return nullptr;
    }
    
    Bitmap* result = new Bitmap(biHeader.dwWidth, biHeader.dwHeight);
    int rowSize = ((biHeader.dwBitsPerPixel * biHeader.dwWidth + 31) / 32) * 4;
    
    uint8_t rowBuffer[rowSize];
    
    for (int y = 0; y < biHeader.dwHeight; y++)
    {
        fseek(fp, header.dwDataOffset + y * rowSize, SEEK_SET);
        if (fread(&rowBuffer, 1, rowSize, fp) != rowSize)
        {
            printf("Error reading row %d from file %s\n", y, filename);
            fclose(fp);
            return nullptr;
        }

        for (int x = 0; x < rowSize; x++)
        {
            switch (biHeader.dwBitsPerPixel)
            {
                case 24:
                {
                    int b = rowBuffer[x * 3];
                    int g = rowBuffer[x * 3 + 1];
                    int r = rowBuffer[x * 3 + 2];
                    ARGBPixel pixel = PACK_PIXEL(r, g, b);
                    result->setPixel(x, biHeader.dwHeight - y - 1, pixel);
                    break;
                }
                default:
                {
                    // TODO support other pixel formats if needed
                    break;
                }
            }
        }
    }
    
    fclose(fp);
    return result;
}

void Bitmap::save (Bitmap* bitmap, const char* filename)
{
}
