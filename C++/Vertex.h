//
//  Vertex.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <vector>
#include "Vector4.h"

namespace MiniVideoCard
{
    using namespace std;
    
    class Vertex
    {
    public:
        Vector4& Position();
        
        vector<double>& Varyings();
        
    private:
        Vector4 position;
        
        vector<double> varyings;
    };
}
