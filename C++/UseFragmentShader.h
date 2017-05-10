//
//  UseFragmentShader.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include "Command.h"

namespace MiniVideoCard
{
    class FragmentShader;
    
    class UseFragmentShader : public Command
    {
    public:
        UseFragmentShader(bool deleteAfterUse, FragmentShader* fragmentShader);
        
        FragmentShader* FragmentShader();
        
        ~UseFragmentShader();
        
    private:
        class FragmentShader* fragmentShader;
        
        bool deleteAfterUse;
    };
}
