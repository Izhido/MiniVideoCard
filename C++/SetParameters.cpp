//
//  SetParameters.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/23/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "SetParameters.h"
#include "Parameters.h"

namespace MiniVideoCard
{
    SetParameters::SetParameters()
    {
        this->parameters = nullptr;
        this->deleteAfterUse = false;
    }
    
    SetParameters::SetParameters(bool deleteAfterUse, class Parameters* parameters)
    {
        this->parameters = parameters;
        this->deleteAfterUse = deleteAfterUse;
    }
    
    Parameters* SetParameters::Parameters()
    {
        return parameters;
    }
    
    SetParameters::~SetParameters()
    {
        if (deleteAfterUse)
        {
            delete parameters;
        }
    }
}
