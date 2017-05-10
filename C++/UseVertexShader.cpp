//
//  UseVertexShader.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "UseVertexShader.h"
#include "VertexShader.h"

namespace MiniVideoCard
{
    UseVertexShader::UseVertexShader(bool deleteAfterUse, class VertexShader* vertexShader)
    {
        this->vertexShader = vertexShader;
        this->deleteAfterUse = deleteAfterUse;
    }
    
    VertexShader* UseVertexShader::VertexShader()
    {
        return vertexShader;
    }
    
    UseVertexShader::~UseVertexShader()
    {
        if (deleteAfterUse)
        {
            delete vertexShader;
        }
    }
}
