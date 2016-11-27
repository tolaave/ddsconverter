//
//  DDSDecompressor.hpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#ifndef DDSDecompressor_hpp
#define DDSDecompressor_hpp

#include "Bitmap.hpp"

#include <stdlib.h>

class DDSDecompressor
{
public:
            DDSDecompressor (const char* sourcefile);
    Bitmap* decompress      ();
    
private:
    
};

#endif /* DDSDecompressor_hpp */
