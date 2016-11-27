//
//  Bitmap.cpp
//  ddsconverter
//
//  Created by Toni Lääveri on 27.11.2016.
//  Copyright © 2016 Toni Lääveri. All rights reserved.
//

#include "Bitmap.hpp"

Bitmap::Bitmap (int width, int height) :
    m_width(width),
    m_height(height)
{
    m_data = new ARGBPixel[width * height];
}

void Bitmap::setPixel(int x, int y, ARGBPixel value)
{
    if (x >= 0 && y >= 0 && x < m_width && y < m_height)
    {
        m_data[y * m_height + m_width] = value;
    }
}

ARGBPixel Bitmap::getPixel (int x, int y)
{
    if (x >= 0 && y >= 0 && x < m_width && y < m_height)
    {
        return m_data[y * m_height + m_width];
    }
    return 0;
}