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

typedef uint32_t ARGBPixel;

class Bitmap
{
public:
                Bitmap      (int width, int height);
    void        setPixel    (int x, int y, ARGBPixel value);
    ARGBPixel   getPixel    (int x, int y);
    
private:
    int         m_width;
    int         m_height;
    ARGBPixel*  m_data;
    
};

#endif /* Bitmap_hpp */
