#ifndef DDSFormat_h
#define DDSFormat_h

#include <stdint.h>

#define BLOCK_WIDTH         4
#define BLOCK_HEIGHT        4
#define BLOCK_SIZE          (BLOCK_WIDTH * BLOCK_HEIGHT)

#define DDPF_FOURCC         0x00000004
    
    typedef struct __attribute__((packed))
    {
        uint32_t            dwSize;
        uint32_t            dwFlags;
        uint32_t            dwFourCC;
        uint32_t            dwRGBBitCount;
        uint32_t            dwRBitMask;
        uint32_t            dwGBitMask;
        uint32_t            dwBBitMask;
        uint32_t            dwABitMask;
    } DDSPixelFormat;
    
#define DDSD_CAPS           0x00000001
#define DDSD_HEIGHT         0x00000002
#define DDSD_WIDTH          0x00000004
#define DDSD_PIXELFORMAT    0x00001000
    
    typedef struct __attribute__((packed))
    {
        uint32_t            dwMagic;
        uint32_t            dwSize;
        uint32_t            dwFlags;
        uint32_t            dwHeight;
        uint32_t            dwWidth;
        uint32_t            dwPitchOrLinearSize;
        uint32_t            dwDepth;
        uint32_t            dwMipMapCount;
        uint32_t            dwReserved[11];
        DDSPixelFormat      ddspf;
        uint32_t            dwCaps;
        uint32_t            dwCaps2;
        uint32_t            dwCaps3;
        uint32_t            dwCaps4;
        uint32_t            dwReserved2;
    } DDSHeader;

#endif /* DDSFormat_h */