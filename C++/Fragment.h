//
//  Fragment.h
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
    
    class Fragment
    {
    public:
        Fragment(double x, double y, double z, size_t varyingCount, bool discarded);

        const Vector3& Position() const;
        
        vector<double>& Varyings();

        vector<double>* VaryingsAtRight();
        
        void SetVaryingsAtRight(vector<double>* varyingsAtRight);
        
        vector<double>* VaryingsBelow();
        
        void SetVaryingsBelow(vector<double>* varyingsBelow);
        
        Vector4& Color();

        bool Discarded();
        
        void Discard();
        
        void Set(double x, double y, double z, size_t varyingCount, bool discarded);
        
        double DerivativeX(size_t index);
        
        double DerivativeY(size_t index);
        
    private:
        Vector3 position;
        
        vector<double> varyings;
        
        vector<double>* varyingsAtRight;
        
        vector<double>* varyingsBelow;
        
        Vector4 color;
        
        bool discarded;
    };
}
