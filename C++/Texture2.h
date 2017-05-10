//
//  Texture2.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/29/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <cstddef>
#include <vector>
#include "Vector2.h"
#include "Fragment.h"
#include "Filter.h"
#include "Wrap.h"
#include "Mipmap2.h"

namespace MiniVideoCard
{
    using namespace std;
    
    class Rasterizer;
	class Sampler2;

    class Texture2
    {
    public:
        Texture2(size_t width, size_t height, size_t components);
        
        Texture2(size_t width, size_t height, size_t components, bool mipmapped);
        
        Texture2(const Texture2& source);

        Texture2(Rasterizer& rasterizer);
        
        Texture2(Rasterizer& rasterizer, bool mipmapped, Sampler2* sampler);
        
        Texture2(Rasterizer& rasterizer, bool fromDepthBuffer);
        
        Texture2(Rasterizer& rasterizer, bool fromDepthBuffer, bool mipmapped, Sampler2* sampler);
        
        size_t Width();
        
        size_t Width(size_t level);
        
        size_t Height();
        
        size_t Height(size_t level);

        size_t Offset(size_t level);
        
        size_t Components();
        
        bool Mipmapped();

        size_t Levels();
        
        vector<double>& Buffer();
        
    private:
        size_t components;
        
        bool mipmapped;

        vector<Mipmap2> mipmaps;

        vector<double> buffer;
    };
}
