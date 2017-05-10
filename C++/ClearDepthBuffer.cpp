//
//  ClearDepthBuffer.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "ClearDepthBuffer.h"

namespace MiniVideoCard
{
    ClearDepthBuffer::ClearDepthBuffer()
    {
        this->depth = 1;
    }
    
    ClearDepthBuffer::ClearDepthBuffer(double depth)
    {
        this->depth = depth;
    }
    
    double ClearDepthBuffer::Depth()
    {
        return depth;
    }
}
