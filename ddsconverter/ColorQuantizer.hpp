//
//  ColorQuantizer.hpp
//  ddsconverter
//
//  Created by Toni Lääveri on 29.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#ifndef ColorQuantizer_hpp
#define ColorQuantizer_hpp

#include <vector>
#include <utility>
#include <array>

#include "Bitmap.hpp"

class ColorQuantizer
{
public:
            ColorQuantizer      ();
    
    void    quantize            (std::vector<ARGBPixel> sourceColors, std::pair<ARGBPixel,ARGBPixel> outputColors);
    
private:
    void    pickMinMaxLuminance (std::vector<ARGBPixel> colors, ARGBPixel* minRGB, ARGBPixel* maxRGB);

};

#endif /* ColorQuantizer_hpp */
