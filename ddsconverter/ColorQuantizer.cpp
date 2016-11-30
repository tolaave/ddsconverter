//
//  ColorQuantizer.cpp
//  ddsconverter
//
//  Created by Toni Lääveri on 29.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#include "ColorQuantizer.hpp"

ColorQuantizer::ColorQuantizer()
{
}

void ColorQuantizer::quantize(std::vector<ARGBPixel> sourceColors, std::pair<ARGBPixel,ARGBPixel> outputColors)
{
    // Pick the RGB values in source colors with lowest and highest luminance values
    // as starting points for quantization
    std::array<ARGBPixel, 2> minMaxColors;
    pickMinMaxLuminance(sourceColors, &minMaxColors[0], &minMaxColors[1]);
    
    // Create also interpolated RGB values representing the colors 2 and 3 in the final DDS compression
    std::array<ARGBPixel, 4> testColors =
    {
        minMaxColors[0],
        INTERPOLATE_RGB(minMaxColors[0], minMaxColors[1], 1, 2),
        INTERPOLATE_RGB(minMaxColors[0], minMaxColors[1], 2, 1),
        minMaxColors[1]
    };
    
    std::array<int, 2> deltaR = {0, 0};
    std::array<int, 2> deltaG = {0, 0};
    std::array<int, 2> deltaB = {0, 0};
    std::array<int, 2> deltaWeight = {0, 0};
    
    for (std::vector<ARGBPixel>::iterator it = sourceColors.begin(); it != sourceColors.end(); it++)
    {
        ARGBPixel pixel = *it;
        uint8_t r = GET_R(pixel);
        uint8_t g = GET_G(pixel);
        uint8_t b = GET_B(pixel);
        
        int diffRbest = 0;
        int diffGbest = 0;
        int diffBbest = 0;
        int bestTestColor = 0;
        
        float diffVecBest = (float)INT32_MAX;
        
        for (int i = 0; i < testColors.size(); i++)
        {
            ARGBPixel color = testColors[i];
            uint8_t rt = GET_R(color);
            uint8_t gt = GET_G(color);
            uint8_t bt = GET_B(color);
            
            int diffR = (int)r - (int)rt;
            int diffG = (int)g - (int)gt;
            int diffB = (int)b - (int)bt;
            
            float diffVec = (diffR * diffR) * LUMINANCE_R + (diffG * diffG) * LUMINANCE_G + (diffB * diffB) * LUMINANCE_B;
            if (diffVec < diffVecBest)
            {
                diffVecBest = diffVec;
                diffRbest = diffR;
                diffGbest = diffG;
                diffBbest = diffB;
                bestTestColor = i;
            }
        }
        
        deltaR[0] += diffRbest * (3 - bestTestColor);
        deltaG[0] += diffGbest * (3 - bestTestColor);
        deltaB[0] += diffBbest * (3 - bestTestColor);
        deltaWeight[0] += 3 - bestTestColor;
        deltaR[1] += diffRbest * (bestTestColor);
        deltaG[1] += diffGbest * (bestTestColor);
        deltaB[1] += diffBbest * (bestTestColor);
        deltaWeight[1] += bestTestColor;
    }
    
    for (int i = 0; i < 2; i++)
    {
        if (deltaWeight[i] == 0)
            deltaWeight[i] = 1;
        
        ARGBPixel color = minMaxColors[i];
        uint8_t r = GET_R(color);
        uint8_t g = GET_G(color);
        uint8_t b = GET_B(color);
        
        r = std::min(255, std::max(0, r + deltaR[i] / deltaWeight[i]));
        g = std::min(255, std::max(0, g + deltaR[i] / deltaWeight[i]));
        b = std::min(255, std::max(0, b + deltaR[i] / deltaWeight[i]));
        
        minMaxColors[i] = PACK_PIXEL(r, g, b);
    }
    
    outputColors.first = minMaxColors[0];
    outputColors.second = minMaxColors[1];
}

void ColorQuantizer::pickMinMaxLuminance(std::vector<ARGBPixel> colors, ARGBPixel* minRGB, ARGBPixel* maxRGB)
{
    float minVecLen = (float)UINT32_MAX;
    float maxVecLen = 0;
    
    for (std::vector<ARGBPixel>::iterator it = colors.begin(); it != colors.end(); it++)
    {
        ARGBPixel pixel = *it;
        uint8_t r = GET_R(pixel);
        uint8_t g = GET_G(pixel);
        uint8_t b = GET_B(pixel);
        
        float squaredLuminanceVectorLength = r * r * LUMINANCE_R + g * g * LUMINANCE_G + b * b * LUMINANCE_B;
        
        if (squaredLuminanceVectorLength <= minVecLen)
        {
            minVecLen = squaredLuminanceVectorLength;
            *minRGB = pixel;
        }
        if (squaredLuminanceVectorLength >= maxVecLen)
        {
            maxVecLen = squaredLuminanceVectorLength;
            *maxRGB = pixel;
        }
    }
}
