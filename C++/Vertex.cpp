//
//  Vertex.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Vertex.h"

namespace MiniVideoCard
{
    using namespace std;

    Vector4& Vertex::Position()
    {
        return position;
    }

    vector<double>& Vertex::Varyings()
    {
        return varyings;
    }
}
