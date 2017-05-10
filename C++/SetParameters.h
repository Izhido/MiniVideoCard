//
//  SetParameters.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/23/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include "Command.h"

namespace MiniVideoCard
{
    class Parameters;
    
    class SetParameters : public Command
    {
    public:
        SetParameters();
        
        SetParameters(bool deleteAfterUse, Parameters* parameters);
        
        Parameters* Parameters();
        
        ~SetParameters();
        
    private:
        class Parameters* parameters;
        
        bool deleteAfterUse;
    };
}
