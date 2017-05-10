//
//  UseVertexShader.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include "Command.h"

namespace MiniVideoCard
{
    class VertexShader;
    
    class UseVertexShader : public Command
    {
    public:
        UseVertexShader(bool deleteAfterUse, VertexShader* vertexShader);
        
        VertexShader* VertexShader();
        
        ~UseVertexShader();

    private:
        class VertexShader* vertexShader;
        
        bool deleteAfterUse;
    };
}
