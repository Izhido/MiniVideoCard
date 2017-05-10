//
//  ClearColorBuffer.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include "Command.h"

namespace MiniVideoCard
{
    class ClearColorBuffer : public Command
    {
    public:
        ClearColorBuffer();

        ClearColorBuffer(double r, double g, double b);

        ClearColorBuffer(double r, double g, double b, double a);

        double R();
        
        double G();
        
        double B();
        
        double A();
        
    private:
        double r;

        double g;
        
        double b;
        
        double a;
    };
}
