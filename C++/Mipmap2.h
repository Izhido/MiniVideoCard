//
//  Mipmap2.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/29/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once
#include <cstddef>

namespace MiniVideoCard
{
    using namespace std;
    
    class Mipmap2
    {
    public:
        Mipmap2(size_t width, size_t height, size_t offset);

        size_t Width();
        
        size_t Height();
        
        size_t Offset();

    private:
        size_t width;
        
        size_t height;
        
        size_t offset;
    };
}
