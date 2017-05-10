//
//  UseFragmentShader.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "UseFragmentShader.h"
#include "FragmentShader.h"

namespace MiniVideoCard
{
    UseFragmentShader::UseFragmentShader(bool deleteAfterUse, class FragmentShader* fragmentShader)
    {
        this->fragmentShader = fragmentShader;
        this->deleteAfterUse = deleteAfterUse;
    }
    
    FragmentShader* UseFragmentShader::FragmentShader()
    {
        return fragmentShader;
    }
    
    UseFragmentShader::~UseFragmentShader()
    {
        if (deleteAfterUse)
        {
            delete fragmentShader;
        }
    }
}
