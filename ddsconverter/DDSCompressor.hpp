//
//  DDSCompressor.hpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#ifndef DDSCompressor_hpp
#define DDSCompressor_hpp

#include <array>

#include "ColorQuantizer.hpp"
#include "Bitmap.hpp"
#include "DDSFormat.h"

class DDSCompressor
{
public:
                    DDSCompressor       (Bitmap* bitmap);
    int             compress            (const char* targetfile);
    
private:
    
    DDSBlockData    compressBlock       (int x, int y, FILE* fp);
    int             chooseClosestIndex  (Color32 color, std::array<Color32, 4>& colors);
    
    Bitmap*         m_bitmap;
    ColorQuantizer  m_quantizer;
    
};

#endif /* DDSCompressor_hpp */
