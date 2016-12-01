//
//  Bitmap.hpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#ifndef Bitmap_hpp
#define Bitmap_hpp

#include <stdint.h>
#include <type_traits>

typedef uint32_t            Color32;
typedef uint16_t            Color16;

#define PACK_C32(r,g,b)     ((Color32)(((r << 16) & 0xff0000) | ((g << 8) & 0xff00) | (b & 0xff)))
#define PACK_C16(r,g,b)     ((Color16)((((r >> 3) & 0x1f) << 11) | (((g >> 2) & 0x3f) << 5) | ((b >> 3) & 0x1f))) 

#define C32_TO_16(c32)      ((Color16)((((c32 >> (16 + 3)) & 0x1f) << 11) | (((c32 >> (8 + 2)) & 0x3f) << 5) | ((c32 >> 3) & 0x1f)))
#define C16_TO_32(c16)      ((Color32)((((c16 >> 11) & 0x1f) << (16 + 3)) | (((c16 >> 5) & 0x3f) << (8 + 2)) | ((c16 & 0x1f) << 3)))

#define C32_ROUND16(c32)    (C16_TO_32(C32_TO_16(c32)))

#define GET_R32(rgb)        ((rgb >> 16) & 0xff)
#define GET_G32(rgb)        ((rgb >> 8) & 0xff)
#define GET_B32(rgb)        ((rgb) & 0xff)

#define GET_R16(rgb)        (((rgb >> 11) & 0x1f) << 3)
#define GET_G16(rgb)        (((rgb >> 5) & 0x3f) << 2)
#define GET_B16(rgb)        (((rgb) & 0x1f) << 3)

#define INTERPOLATE_C32(rgb1,rgb2,w1,w2)    ((Color32)( \
                            (((((((rgb1 >> 16) & 0xff) * (w1)) + (((rgb2 >> 16) & 0xff) * (w2))) / ((w1) + (w2))) & 0xff) << 16) | \
                            (((((((rgb1 >> 8) & 0xff) * (w1)) + (((rgb2 >> 8) & 0xff) * (w2))) / ((w1) + (w2))) & 0xff) << 8) | \
                            ((((((rgb1) & 0xff) * (w1)) + (((rgb2) & 0xff) * (w2))) / ((w1) + (w2))) & 0xff)))

// Source for channel-specific luminance weights: https://en.wikipedia.org/wiki/Relative_luminance
#define LUMINANCE_R         0.2126f
#define LUMINANCE_G         0.7152f
#define LUMINANCE_B         0.0722f

#define SWAP(a,b)           { std::remove_reference<decltype(a)>::type _t; _t = a; a = b; b = _t; }

class Bitmap
{
public:
                    Bitmap      (int width, int height);
    
    int             getWidth    () { return m_width; }
    int             getHeight   () { return m_height; }
    void            setPixel    (int x, int y, Color32 value);
    Color32         getPixel    (int x, int y);
    
    static Bitmap*  load        (const char* filename);
    static void     save        (Bitmap* bitmap, const char* filename);
    
private:
    int             m_width;
    int             m_height;
    Color32*        m_data;
    
};

#endif /* Bitmap_hpp */
