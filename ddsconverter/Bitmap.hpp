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

typedef uint32_t            ARGBPixel;

#define PACK_PIXEL(r,g,b)   ((ARGBPixel)(((r << 16) & 0xff0000) | ((g << 8) & 0xff00) | (b & 0xff)))

#define GET_R(rgb)          ((rgb >> 16) & 0xff)
#define GET_G(rgb)          ((rgb >> 8) & 0xff)
#define GET_B(rgb)          ((rgb >> 0) & 0xff)

#define INTERPOLATE_RGB(rgb1,rgb2,w1,w2)    ((ARGBPixel)(                               \
                            ((((rgb1) & 0xff00ff) * (w1) / ((w1) + (w2))                \
                            + ((rgb2) & 0xff00ff) * (w2) / ((w1) + (w2))) & 0xff00ff) | \
                            ((((rgb1) & 0x00ff00) * (w1) / ((w1) + (w2))                \
                            + ((rgb2) & 0x00ff00) * (w2) / ((w1) + (w2))) & 0x00ff00)))

// Source for channel-specific luminance weights: https://en.wikipedia.org/wiki/Relative_luminance
#define LUMINANCE_R         0.2126f
#define LUMINANCE_G         0.7152f
#define LUMINANCE_B         0.0722f

class Bitmap
{
public:
                    Bitmap      (int width, int height);
    
    int             getWidth    () { return m_width; }
    int             getHeight   () { return m_height; }
    void            setPixel    (int x, int y, ARGBPixel value);
    ARGBPixel       getPixel    (int x, int y);
    
    static Bitmap*  load        (const char* filename);
    static void     save        (Bitmap* bitmap, const char* filename);
    
private:
    int             m_width;
    int             m_height;
    ARGBPixel*      m_data;
    
};

#endif /* Bitmap_hpp */
