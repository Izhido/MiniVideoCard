//
//  ClearColorBuffer.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "ClearColorBuffer.h"

namespace MiniVideoCard
{
    ClearColorBuffer::ClearColorBuffer()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 1;
    }
    
    ClearColorBuffer::ClearColorBuffer(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 1;
    }
    
    ClearColorBuffer::ClearColorBuffer(double r, double g, double b, double a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    
    double ClearColorBuffer::R()
    {
        return r;
    }
    
    double ClearColorBuffer::G()
    {
        return g;
    }
    
    double ClearColorBuffer::B()
    {
        return b;
    }
    
    double ClearColorBuffer::A()
    {
        return a;
    }
}
