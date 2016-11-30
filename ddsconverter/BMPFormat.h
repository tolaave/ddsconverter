//
//  BMPFormat.h
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#ifndef BMPFormat_h
#define BMPFormat_h

#include <stdint.h>

#define BM_TYPE                 ((uint16_t)(((int)'B') | ((int)'M' << 8)))

typedef struct __attribute__((packed))
{
    uint16_t                    wType;
    uint32_t                    dwSize;
    uint16_t                    wReserved1;
    uint16_t                    wReserved2;
    uint32_t                    dwDataOffset;
} BMPHeader;

#define BITMAPINFOHEADER_SIZE   40

// Compression method
#define BI_RGB                  0

typedef struct __attribute__((packed))
{
    uint32_t                    dwSize;
    uint32_t                    dwWidth;
    uint32_t                    dwHeight;
    uint16_t                    dwNumColorPlanes;
    uint16_t                    dwBitsPerPixel;
    uint32_t                    dwCompression;
    uint32_t                    dwImageSize;
    uint32_t                    dwHRes;
    uint32_t                    dwVRes;
    uint32_t                    dwPaletteSize;
    uint32_t                    dwNumImporantColors;
} BitmapInfoHeader;

#endif /* BMPFormat_h */
