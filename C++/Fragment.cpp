//
//  Fragment.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Fragment.h"

namespace MiniVideoCard
{
    using namespace std;
    
    Fragment::Fragment(double x, double y, double z, size_t varyingCount, bool discarded) : position(x, y, z), varyingsAtRight(nullptr), varyingsBelow(nullptr), discarded(discarded)
    {
        if (varyingCount > 0)
        {
            varyings.resize(varyingCount);
        }
    }
    
    const Vector3& Fragment::Position() const
    {
        return position;
    }
    
    vector<double>& Fragment::Varyings()
    {
        return varyings;
    }

    vector<double>* Fragment::VaryingsAtRight()
    {
        return varyingsAtRight;
    }
    
    void Fragment::SetVaryingsAtRight(vector<double>* varyingsAtRight)
    {
        this->varyingsAtRight = varyingsAtRight;
    }
    
    vector<double>* Fragment::VaryingsBelow()
    {
        return varyingsBelow;
    }
    
    void Fragment::SetVaryingsBelow(vector<double>* varyingsBelow)
    {
        this->varyingsBelow = varyingsBelow;
    }
    
    Vector4& Fragment::Color()
    {
        return color;
    }
    
    bool Fragment::Discarded()
    {
        return discarded;
    }
    
    void Fragment::Discard()
    {
        discarded = true;
    }

    double Fragment::DerivativeX(size_t index)
    {
        if (varyingsAtRight == nullptr)
        {
            throw new runtime_error("Varyings at right were not provided.");
        }

        if (index >= varyings.size() || index >= varyingsAtRight->size())
        {
            throw new runtime_error("Index out of range for varyings.");
        }
        
        return varyingsAtRight->at(index) - varyings[index];
    }
    
    double Fragment::DerivativeY(size_t index)
    {
        if (varyingsBelow == nullptr)
        {
            throw new runtime_error("Varyings below were not provided.");
        }
        
        if (index >= varyings.size() || index >= varyingsBelow->size())
        {
            throw new runtime_error("Index out of range for varyings.");
        }
        
        return varyingsBelow->at(index) - varyings[index];
    }
}
