//
//  Texture2.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/29/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Texture2.h"
#include "Rasterizer.h"
#include <cmath>
#include "Sampler2.h"

namespace MiniVideoCard
{
    using namespace std;
    
    Texture2::Texture2(size_t width, size_t height, size_t components) : Texture2(width, height, components, false)
    {
    }
    
    Texture2::Texture2(size_t width, size_t height, size_t components, bool mipmapped)
    {
        if (width == 0)
        {
            throw runtime_error("Width cannot be zero.");
        }
        
        if (height == 0)
        {
            throw runtime_error("Height cannot be zero.");
        }

        if (components < 1)
        {
            throw runtime_error("Components cannot be less than 1.");
        }
        
        if (components > 4)
        {
            throw runtime_error("Components cannot be greater than 4.");
        }
        
        this->components = components;
        
        this->mipmapped = mipmapped;
        
        size_t size = width * height * components;
        
        mipmaps.emplace_back(width, height, 0);
        
        if (mipmapped)
        {
			size_t levelWidth = width;
			size_t levelHeight = height;

			size_t divisor = 2;

            while (levelWidth > 1 || levelHeight > 1)
            {
                levelWidth = width / divisor;
                
                if (levelWidth < 1)
                {
					levelWidth = 1;
                }
                
				levelHeight = height / divisor;
                
                if (levelHeight < 1)
                {
					levelHeight = 1;
                }

                mipmaps.emplace_back(levelWidth, levelHeight, size);
                
                size += levelWidth * levelHeight * components;

				divisor *= 2;
            }
        }
        
        buffer.resize(size);
    }

    Texture2::Texture2(const Texture2& source)
    {
        components = source.components;
        mipmapped = source.mipmapped;
        mipmaps = source.mipmaps;
        buffer = source.buffer;
    }
    
    Texture2::Texture2(Rasterizer& rasterizer) : Texture2(rasterizer, false, false, nullptr)
    {
    }
    
    Texture2::Texture2(Rasterizer& rasterizer, bool mipmapped, Sampler2* sampler) : Texture2(rasterizer, false, mipmapped, sampler)
    {
    }
    
    Texture2::Texture2(Rasterizer& rasterizer, bool fromDepthBuffer) : Texture2(rasterizer, fromDepthBuffer, false, nullptr)
    {
    }
    
    Texture2::Texture2(Rasterizer& rasterizer, bool fromDepthBuffer, bool mipmapped, Sampler2* sampler) : Texture2(rasterizer.Width(), rasterizer.Height(), fromDepthBuffer ? 1 : 4, mipmapped)
    {
        if (fromDepthBuffer)
        {
            copy(rasterizer.DepthBuffer().begin(), rasterizer.ColorBuffer().begin() + Width() * Height() * components, buffer.begin());
        }
        else
        {
            copy(rasterizer.ColorBuffer().begin(), rasterizer.ColorBuffer().begin() + Width() * Height() * components, buffer.begin());
        }
        
        if (mipmapped && sampler != nullptr)
        {
            sampler->GenerateMipmaps(*this);
        }
    }
    
    size_t Texture2::Width()
    {
        return Width(0);
    }
    
    size_t Texture2::Width(size_t level)
    {
        if (level >= mipmaps.size())
        {
            throw runtime_error("Mipmap level out of bounds.");
        }
        
        return mipmaps[level].Width();
    }
    
    size_t Texture2::Height()
    {
        return Height(0);
    }
    
    size_t Texture2::Height(size_t level)
    {
        if (level >= mipmaps.size())
        {
            throw runtime_error("Mipmap level out of bounds.");
        }
        
        return mipmaps[level].Height();
    }
    
    size_t Texture2::Offset(size_t level)
    {
        if (level >= mipmaps.size())
        {
            throw runtime_error("Mipmap level out of bounds.");
        }
        
        return mipmaps[level].Offset();
    }

    size_t Texture2::Components()
    {
        return components;
    }
    
    bool Texture2::Mipmapped()
    {
        return mipmapped;
    }
    
    size_t Texture2::Levels()
    {
        return mipmaps.size();
    }
    
    vector<double>& Texture2::Buffer()
    {
        return buffer;
    }
    
}
