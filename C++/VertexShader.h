//
//  VertexShader.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

namespace MiniVideoCard
{
    class Parameters;
    class Data;
    class Vertex;
    
    class VertexShader
    {
    public:
        virtual void Run(Data* data, Vertex* vertex, Parameters* parameters) = 0;
        
        virtual ~VertexShader();
    };
}
