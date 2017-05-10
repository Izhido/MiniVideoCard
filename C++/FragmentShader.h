//
//  FragmentShader.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

namespace MiniVideoCard
{
    class Fragment;
    class Parameters;
    
    class FragmentShader
    {
    public:
        virtual void Run(Fragment* fragment, Parameters* parameters) = 0;
        
        virtual ~FragmentShader();
    };
}
