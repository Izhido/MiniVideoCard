//
//  ClearDepthBuffer.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include "Command.h"

namespace MiniVideoCard
{
    class ClearDepthBuffer : public Command
    {
    public:
        ClearDepthBuffer();
        
        ClearDepthBuffer(double depth);
        
        double Depth();
        
    private:
        double depth;
    };
}
