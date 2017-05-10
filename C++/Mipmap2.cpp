//
//  Mipmap2.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/29/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Mipmap2.h"

namespace MiniVideoCard
{
    using namespace std;
    
    Mipmap2::Mipmap2(size_t width, size_t height, size_t offset)
    {
        this->width = width;
        this->height = height;
        this->offset = offset;
    }
    
    size_t Mipmap2::Width()
    {
        return width;
    }
    
    size_t Mipmap2::Height()
    {
        return height;
    }
    
    size_t Mipmap2::Offset()
    {
        return offset;
    }
}
