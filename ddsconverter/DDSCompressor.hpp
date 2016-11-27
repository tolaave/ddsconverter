//
//  DDSCompressor.hpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#ifndef DDSCompressor_hpp
#define DDSCompressor_hpp

#include "Bitmap.hpp"

class DDSCompressor
{
public:
            DDSCompressor   (Bitmap* bitmap);
    int     compress        (const char* targetfile);
    
private:
    
};

#endif /* DDSCompressor_hpp */