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
    
    void    quantize            (std::vector<Color32> sourceColors, std::pair<Color32,Color32>& outputColors);
    
private:
    void    pickMinMaxLuminance (std::vector<Color32> colors, Color32* minRGB, Color32* maxRGB);

};

#endif /* ColorQuantizer_hpp */
