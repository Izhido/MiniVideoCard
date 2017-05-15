//
//  Sampler2.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/29/17.
//  Copyright � 2017 Heriberto Delgado. All rights reserved.
//

#include "Sampler2.h"
#include <cmath>
#include <algorithm>

namespace MiniVideoCard
{
    using namespace std;
    
    Filter Sampler2::MinificationFilter()
    {
        return minificationFilter;
    }
    
    void Sampler2::SetMinificationFilter(Filter filter)
    {
        minificationFilter = filter;
    }
    
    Filter Sampler2::MagnificationFilter()
    {
        return magnificationFilter;
    }
    
    void Sampler2::SetMagnificationFilter(Filter filter)
    {
        magnificationFilter = filter;
    }
    
    Wrap Sampler2::HorizontalWrap()
    {
        return horizontalWrap;
    }
    
    void Sampler2::SetHorizontalWrap(Wrap wrap)
    {
        horizontalWrap = wrap;
    }
    
    Wrap Sampler2::VerticalWrap()
    {
        return verticalWrap;
    }
    
    void Sampler2::SetVerticalWrap(Wrap wrap)
    {
        verticalWrap = wrap;
    }
    
    Vector4& Sampler2::Border()
    {
        return border;
    }
    
    void Sampler2::GenerateMipmaps(Texture2& texture)
    {
        if (!texture.Mipmapped())
        {
            throw runtime_error("No mipmap levels present.");
        }
        
        auto previousWidth = (double)texture.Width(0);
        auto previousHeight = (double)texture.Height(0);
        auto previousOffset = texture.Offset(0);
        
        auto components = texture.Components();
        
        auto& buffer = texture.Buffer();
        
        for (size_t level = 1; level < texture.Levels(); level++)
        {
            auto width = (double)texture.Width(level);
            auto height = (double)texture.Height(level);
            auto offset = texture.Offset(level);
            
            auto horizontal = previousWidth / width;
            auto vertical = previousHeight / height;
            
            auto halfHorizontal = horizontal / 2;
            auto halfVertical = vertical / 2;
            
            auto endX = previousWidth + 1;
            auto endY = previousHeight + 1;
            
            size_t target = 0;
            
            if (minificationFilter == NearestNeighbor)
            {
                for (auto y = halfVertical; y < endY; y += vertical)
                {
                    for (auto x = halfHorizontal; x < endX; x += horizontal)
                    {
                        auto bufferX = (size_t)floor(x);
                        auto bufferY = (size_t)floor(y);
                        
                        size_t position = bufferY * (size_t)previousWidth * components + bufferX * components;
                        
                        for (size_t i = 0; i < components; i++)
                        {
                            buffer[offset + target] = buffer[previousOffset + position + i];
                            
                            target++;
                        }
                    }
                }
            }
            else if (width > 1 && height > 1)
            {
                for (auto y = halfVertical; y < endY; y += vertical)
                {
                    for (auto x = halfHorizontal; x < endX; x += horizontal)
                    {
                        auto topLeftX = (size_t)floor(x - 0.5);
                        auto topLeftY = (size_t)floor(y - 0.5);
                        
                        auto bottomRightX = (size_t)floor(x + 0.5);
                        auto bottomRightY = (size_t)floor(y + 0.5);
                        
                        size_t topLeftPosition = previousOffset + (topLeftY * (size_t)previousWidth + topLeftX) * components;
                        
                        size_t topRightPosition = previousOffset + (topLeftY * (size_t)previousWidth + bottomRightX) * components;
                        
                        size_t bottomRightPosition = previousOffset + (bottomRightY * (size_t)previousWidth + bottomRightX) * components;
                        
                        size_t bottomLeftPosition = previousOffset + (bottomRightY * (size_t)previousWidth + topLeftX) * components;
                        
                        for (size_t i = 0; i < components; i++)
                        {
                            buffer[offset + target] = buffer[topLeftPosition + i] * 0.25 + buffer[topRightPosition + i] * 0.25 + buffer[bottomRightPosition + i] * 0.25 + buffer[bottomLeftPosition + i] * 0.25;
                            
                            target++;
                        }
                    }
                }
            }
            else
            {
                for (auto y = halfVertical; y < endY; y += vertical)
                {
                    for (auto x = halfHorizontal; x < endX; x += horizontal)
                    {
                        auto topLeftX = (size_t)floor(x - 0.5);
                        auto topLeftY = (size_t)floor(y - 0.5);
                        
                        auto bottomRightX = (size_t)floor(x + 0.5);
                        auto bottomRightY = (size_t)floor(y + 0.5);
                        
                        size_t topLeftPosition = previousOffset + (topLeftY * (size_t)previousWidth + topLeftX) * components;
                        
                        size_t topRightPosition = previousOffset + (topLeftY * (size_t)previousWidth + bottomRightX) * components;

                        size_t bottomRightPosition = previousOffset + (bottomRightY * (size_t)previousWidth + bottomRightX) * components;
                        
                        size_t bottomLeftPosition = previousOffset + (bottomRightY * (size_t)previousWidth + topLeftX) * components;
                        
                        bool rightOutOfBounds = (bottomRightX >= (size_t)previousWidth);
                        
                        bool bottomOutOfBounds = (bottomRightY >= (size_t)previousHeight);
                        
                        auto weight = 0.25;
                        
                        if (rightOutOfBounds)
                        {
                            weight *= 2;
                        }
                        
                        if (bottomOutOfBounds)
                        {
                            weight *= 2;
                        }
                        
                        for (size_t i = 0; i < components; i++)
                        {
                            auto value = buffer[topLeftPosition + i] * weight;
                            
                            if (!rightOutOfBounds)
                            {
                                value += buffer[topRightPosition + i] * weight;
                            }
                            
                            if (!rightOutOfBounds && !bottomOutOfBounds)
                            {
                                value += buffer[bottomRightPosition + i] * weight;
                            }
                            
                            if (!bottomOutOfBounds)
                            {
                                value += buffer[bottomLeftPosition + i] * weight;
                            }
                            
                            buffer[offset + target] = value;
                            
                            target++;
                        }
                    }
                }
            }

            previousWidth = width;
            previousHeight = height;
            previousOffset = offset;
        }
    }
    
    void Sampler2::FilterAndLevelFor(Texture2& texture, Vector2& position, Fragment& fragment, Filter& filter, size_t& level)
    {
        filter = magnificationFilter;
        
        level = 0;
        
        if (position.Source() == &fragment.Varyings())
        {
            auto width = (double)texture.Width();
            auto height = (double)texture.Height();
            
            auto offsetX = position.Offset();
            auto offsetY = offsetX + 1;
            
            auto dxdx = fragment.DerivativeX(offsetX);
            auto dydx = fragment.DerivativeX(offsetY);
            auto dxdy = fragment.DerivativeY(offsetX);
            auto dydy = fragment.DerivativeY(offsetY);
            
            auto deltaXX = dxdx * width;
            auto deltaXY = dxdy * width;
            auto deltaYY = dydy * height;
            auto deltaYX = dydx * height;
            
            auto squareLengthX = deltaXX * deltaXX + deltaYX * deltaYX;
            auto squareLengthY = deltaXY * deltaXY + deltaYY * deltaYY;
            
            auto calculatedLevel = floor(0.5 * log2(max(squareLengthX, squareLengthY)));

            if (calculatedLevel >= 0)
            {
                filter = minificationFilter;
                
                if (calculatedLevel >= (double)texture.Levels())
                {
                    level = texture.Levels() - 1;
                }
                else
                {
                    level = (size_t)calculatedLevel;
                }
            }
        }
    }

    void Sampler2::CoordinateFor(double raw, double& coordinate, bool& outOfBounds, bool& inverted, double size, Wrap wrap)
    {
        outOfBounds = false;
        inverted = false;
        
        if (wrap == Repeat)
        {
            coordinate = size * (raw - floor(raw));
        }
        else if (wrap == MirroredRepeat)
        {
            if (fmod(floor(raw), 2) == 0)
            {
                coordinate = size * (raw - floor(raw));
            }
            else
            {
                inverted = true;
                
                if (raw >= 0)
                {
                    coordinate = size * (-raw - floor(-raw));
                }
                else
                {
                    coordinate = -(size * (raw - trunc(raw)));
                }
            }
        }
        else if (raw < 0)
        {
            coordinate = raw * size - floor(raw * size);
            
            outOfBounds = true;
        }
        else if (raw >= 1)
        {
            coordinate = size - 1 + raw * size - floor(raw * size);
            
            outOfBounds = true;
        }
        else
        {
            coordinate = raw * size;
        }
    }

    void Sampler2::CoordinatesFor(double s, double t, double& x, double& y, bool& xOutOfBounds, bool& yOutOfBounds, bool& xInverted, bool& yInverted, double width, double height)
    {
        CoordinateFor(s, x, xOutOfBounds, xInverted, width, horizontalWrap);
        
        CoordinateFor(t, y, yOutOfBounds, yInverted, height, verticalWrap);
    }

    double Sampler2::Sample(Texture2& texture, Vector2& position, Fragment& fragment)
    {
        auto components = texture.Components();
        
        if (components != 1)
        {
            throw runtime_error("Components must be 1.");
        }
        
        Filter filter;
        
        size_t level;
        
        FilterAndLevelFor(texture, position, fragment, filter, level);
        
        auto& buffer = texture.Buffer();
        
        auto width = texture.Width(level);
        auto height = texture.Height(level);
        auto offset = texture.Offset(level);
        
        auto widthAsFloatingPoint = (double)width;
        auto heightAsFloatingPoint = (double)height;
        
        auto s = position.X();
        auto t = position.Y();
        
        if (filter == NearestNeighbor)
        {
            double x;
            double y;
            
            bool xOutOfBounds;
            bool yOutOfBounds;
            bool xInverted;
            bool yInverted;
            
            CoordinatesFor(s, t, x, y, xOutOfBounds, yOutOfBounds, xInverted, yInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            if ((horizontalWrap == ClampToBorder && xOutOfBounds) || (verticalWrap == ClampToBorder && yOutOfBounds))
            {
                return border.X();
            }
            else
            {
                size_t position = offset + ((size_t)floor(y) * width + (size_t)floor(x)) * components;
                
                return buffer[position];
            }
        }
        else
        {
            auto halfHorizontal = 1.0 / (widthAsFloatingPoint * 2);
            auto halfVertical = 1.0 / (heightAsFloatingPoint * 2);
            
            auto topLeftS = s - halfHorizontal;
            auto topLeftT = t - halfVertical;
            
            double topLeftX;
            double topLeftY;
            
            bool topLeftXOutOfBounds;
            bool topLeftYOutOfBounds;
            bool topLeftXInverted;
            bool topLeftYInverted;
            
            CoordinatesFor(topLeftS, topLeftT, topLeftX, topLeftY, topLeftXOutOfBounds, topLeftYOutOfBounds, topLeftXInverted, topLeftYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            auto bottomRightS = s + halfHorizontal;
            auto bottomRightT = t + halfVertical;
            
            double bottomRightX;
            double bottomRightY;
            
            bool bottomRightXOutOfBounds;
            bool bottomRightYOutOfBounds;
            bool bottomRightXInverted;
            bool bottomRightYInverted;
            
            CoordinatesFor(bottomRightS, bottomRightT, bottomRightX, bottomRightY, bottomRightXOutOfBounds, bottomRightYOutOfBounds, bottomRightXInverted, bottomRightYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            double topLeft;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topLeft = border.X();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(topLeftX)) * components;
                
                topLeft = buffer[position];
            }
            
            double topRight;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topRight = border.X();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(bottomRightX)) * components;
                
                topRight = buffer[position];
            }
            
            double bottomRight;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomRight = border.X();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(bottomRightX)) * components;
                
                bottomRight = buffer[position];
            }
            
            double bottomLeft;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomLeft = border.X();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(topLeftX)) * components;
                
                bottomLeft = buffer[position];
            }
            
            double leftWeight;
            
            if (topLeftXInverted)
            {
                leftWeight = topLeftX - floor(topLeftX);
            }
            else
            {
                leftWeight = 1 - topLeftX + floor(topLeftX);
            }
            
            double topWeight;
            
            if (topLeftYInverted)
            {
                topWeight = topLeftY - floor(topLeftY);
            }
            else
            {
                topWeight = 1 - topLeftY + floor(topLeftY);
            }
            
            double rightWeight;
            
            if (bottomRightXInverted)
            {
                rightWeight = 1 - bottomRightX + floor(bottomRightX);
            }
            else
            {
                rightWeight = bottomRightX - floor(bottomRightX);
            }
            
            double bottomWeight;
            
            if (bottomRightYInverted)
            {
                bottomWeight = 1 - bottomRightY + floor(bottomRightY);
            }
            else
            {
                bottomWeight = bottomRightY - floor(bottomRightY);
            }
            
            return topLeft * leftWeight * topWeight + topRight * rightWeight * topWeight + bottomRight * bottomWeight * rightWeight + bottomLeft * bottomWeight * leftWeight;
        }
    }
    
    void Sampler2::Sample(Texture2& texture, Vector2& position, Fragment& fragment, Vector2& result)
    {
        auto components = texture.Components();
        
        if (components != 2)
        {
            throw runtime_error("Components must be 2.");
        }
        
        Filter filter;
        
        size_t level;
        
        FilterAndLevelFor(texture, position, fragment, filter, level);
        
        auto& buffer = texture.Buffer();
        
        auto width = texture.Width(level);
        auto height = texture.Height(level);
        auto offset = texture.Offset(level);
        
        auto widthAsFloatingPoint = (double)width;
        auto heightAsFloatingPoint = (double)height;
        
        auto s = position.X();
        auto t = position.Y();
        
        if (filter == NearestNeighbor)
        {
            double x;
            double y;
            
            bool xOutOfBounds;
            bool yOutOfBounds;
            bool xInverted;
            bool yInverted;
            
            CoordinatesFor(s, t, x, y, xOutOfBounds, yOutOfBounds, xInverted, yInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            if ((horizontalWrap == ClampToBorder && xOutOfBounds) || (verticalWrap == ClampToBorder && yOutOfBounds))
            {
                result.Set(border.X(), border.Y());
            }
            else
            {
                size_t position = offset + ((size_t)floor(y) * width + (size_t)floor(x)) * components;
                
                result.Set(buffer[position], buffer[position + 1]);
            }
        }
        else
        {
            auto halfHorizontal = 1.0 / (widthAsFloatingPoint * 2);
            auto halfVertical = 1.0 / (heightAsFloatingPoint * 2);
            
            auto topLeftS = s - halfHorizontal;
            auto topLeftT = t - halfVertical;
            
            double topLeftX;
            double topLeftY;
            
            bool topLeftXOutOfBounds;
            bool topLeftYOutOfBounds;
            bool topLeftXInverted;
            bool topLeftYInverted;
            
            CoordinatesFor(topLeftS, topLeftT, topLeftX, topLeftY, topLeftXOutOfBounds, topLeftYOutOfBounds, topLeftXInverted, topLeftYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            auto bottomRightS = s + halfHorizontal;
            auto bottomRightT = t + halfVertical;
            
            double bottomRightX;
            double bottomRightY;
            
            bool bottomRightXOutOfBounds;
            bool bottomRightYOutOfBounds;
            bool bottomRightXInverted;
            bool bottomRightYInverted;
            
            CoordinatesFor(bottomRightS, bottomRightT, bottomRightX, bottomRightY, bottomRightXOutOfBounds, bottomRightYOutOfBounds, bottomRightXInverted, bottomRightYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            double topLeft1;
            double topLeft2;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topLeft1 = border.X();
                topLeft2 = border.Y();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(topLeftX)) * components;
                
                topLeft1 = buffer[position];
                topLeft2 = buffer[position + 1];
            }
            
            double topRight1;
            double topRight2;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topRight1 = border.X();
                topRight2 = border.Y();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(bottomRightX)) * components;
                
                topRight1 = buffer[position];
                topRight2 = buffer[position + 1];
            }
            
            double bottomRight1;
            double bottomRight2;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomRight1 = border.X();
                bottomRight2 = border.Y();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(bottomRightX)) * components;
                
                bottomRight1 = buffer[position];
                bottomRight2 = buffer[position + 1];
            }
            
            double bottomLeft1;
            double bottomLeft2;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomLeft1 = border.X();
                bottomLeft2 = border.Y();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(topLeftX)) * components;
                
                bottomLeft1 = buffer[position];
                bottomLeft2 = buffer[position + 1];
            }
            
            double leftWeight;
            
            if (topLeftXInverted)
            {
                leftWeight = topLeftX - floor(topLeftX);
            }
            else
            {
                leftWeight = 1 - topLeftX + floor(topLeftX);
            }
            
            double topWeight;
            
            if (topLeftYInverted)
            {
                topWeight = topLeftY - floor(topLeftY);
            }
            else
            {
                topWeight = 1 - topLeftY + floor(topLeftY);
            }
            
            double rightWeight;
            
            if (bottomRightXInverted)
            {
                rightWeight = 1 - bottomRightX + floor(bottomRightX);
            }
            else
            {
                rightWeight = bottomRightX - floor(bottomRightX);
            }
            
            double bottomWeight;
            
            if (bottomRightYInverted)
            {
                bottomWeight = 1 - bottomRightY + floor(bottomRightY);
            }
            else
            {
                bottomWeight = bottomRightY - floor(bottomRightY);
            }
            
            result.Set(topLeft1 * leftWeight * topWeight + topRight1 * rightWeight * topWeight + bottomRight1 * bottomWeight * rightWeight + bottomLeft1 * bottomWeight * leftWeight, topLeft2 * leftWeight * topWeight + topRight2 * rightWeight * topWeight + bottomRight2 * bottomWeight * rightWeight + bottomLeft2 * bottomWeight * leftWeight);
        }
    }
    
    void Sampler2::Sample(Texture2& texture, Vector2& position, Fragment& fragment, Vector3& result)
    {
        auto components = texture.Components();
        
        if (components != 3)
        {
            throw runtime_error("Components must be 3.");
        }
        
        Filter filter;
        
        size_t level;
        
        FilterAndLevelFor(texture, position, fragment, filter, level);
        
        auto& buffer = texture.Buffer();
        
        auto width = texture.Width(level);
        auto height = texture.Height(level);
        auto offset = texture.Offset(level);
        
        auto widthAsFloatingPoint = (double)width;
        auto heightAsFloatingPoint = (double)height;
        
        auto s = position.X();
        auto t = position.Y();
        
        if (filter == NearestNeighbor)
        {
            double x;
            double y;
            
            bool xOutOfBounds;
            bool yOutOfBounds;
            bool xInverted;
            bool yInverted;
            
            CoordinatesFor(s, t, x, y, xOutOfBounds, yOutOfBounds, xInverted, yInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            if ((horizontalWrap == ClampToBorder && xOutOfBounds) || (verticalWrap == ClampToBorder && yOutOfBounds))
            {
                result.Set(border.X(), border.Y(), border.Z());
            }
            else
            {
                size_t position = offset + ((size_t)floor(y) * width + (size_t)floor(x)) * components;
                
                result.Set(buffer[position], buffer[position + 1], buffer[position + 2]);
            }
        }
        else
        {
            auto halfHorizontal = 1.0 / (widthAsFloatingPoint * 2);
            auto halfVertical = 1.0 / (heightAsFloatingPoint * 2);
            
            auto topLeftS = s - halfHorizontal;
            auto topLeftT = t - halfVertical;
            
            double topLeftX;
            double topLeftY;
            
            bool topLeftXOutOfBounds;
            bool topLeftYOutOfBounds;
            bool topLeftXInverted;
            bool topLeftYInverted;
            
            CoordinatesFor(topLeftS, topLeftT, topLeftX, topLeftY, topLeftXOutOfBounds, topLeftYOutOfBounds, topLeftXInverted, topLeftYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            auto bottomRightS = s + halfHorizontal;
            auto bottomRightT = t + halfVertical;
            
            double bottomRightX;
            double bottomRightY;
            
            bool bottomRightXOutOfBounds;
            bool bottomRightYOutOfBounds;
            bool bottomRightXInverted;
            bool bottomRightYInverted;
            
            CoordinatesFor(bottomRightS, bottomRightT, bottomRightX, bottomRightY, bottomRightXOutOfBounds, bottomRightYOutOfBounds, bottomRightXInverted, bottomRightYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            double topLeft1;
            double topLeft2;
            double topLeft3;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topLeft1 = border.X();
                topLeft2 = border.Y();
                topLeft3 = border.Z();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(topLeftX)) * components;
                
                topLeft1 = buffer[position];
                topLeft2 = buffer[position + 1];
                topLeft3 = buffer[position + 2];
            }
            
            double topRight1;
            double topRight2;
            double topRight3;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topRight1 = border.X();
                topRight2 = border.Y();
                topRight3 = border.Z();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(bottomRightX)) * components;
                
                topRight1 = buffer[position];
                topRight2 = buffer[position + 1];
                topRight3 = buffer[position + 2];
            }
            
            double bottomRight1;
            double bottomRight2;
            double bottomRight3;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomRight1 = border.X();
                bottomRight2 = border.Y();
                bottomRight3 = border.Z();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(bottomRightX)) * components;
                
                bottomRight1 = buffer[position];
                bottomRight2 = buffer[position + 1];
                bottomRight3 = buffer[position + 2];
            }
            
            double bottomLeft1;
            double bottomLeft2;
            double bottomLeft3;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomLeft1 = border.X();
                bottomLeft2 = border.Y();
                bottomLeft3 = border.Z();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(topLeftX)) * components;
                
                bottomLeft1 = buffer[position];
                bottomLeft2 = buffer[position + 1];
                bottomLeft3 = buffer[position + 2];
            }
            
            double leftWeight;
            
            if (topLeftXInverted)
            {
                leftWeight = topLeftX - floor(topLeftX);
            }
            else
            {
                leftWeight = 1 - topLeftX + floor(topLeftX);
            }
            
            double topWeight;
            
            if (topLeftYInverted)
            {
                topWeight = topLeftY - floor(topLeftY);
            }
            else
            {
                topWeight = 1 - topLeftY + floor(topLeftY);
            }
            
            double rightWeight;
            
            if (bottomRightXInverted)
            {
                rightWeight = 1 - bottomRightX + floor(bottomRightX);
            }
            else
            {
                rightWeight = bottomRightX - floor(bottomRightX);
            }
            
            double bottomWeight;
            
            if (bottomRightYInverted)
            {
                bottomWeight = 1 - bottomRightY + floor(bottomRightY);
            }
            else
            {
                bottomWeight = bottomRightY - floor(bottomRightY);
            }
            
            result.Set(topLeft1 * leftWeight * topWeight + topRight1 * rightWeight * topWeight + bottomRight1 * bottomWeight * rightWeight + bottomLeft1 * bottomWeight * leftWeight, topLeft2 * leftWeight * topWeight + topRight2 * rightWeight * topWeight + bottomRight2 * bottomWeight * rightWeight + bottomLeft2 * bottomWeight * leftWeight, topLeft3 * leftWeight * topWeight + topRight3 * rightWeight * topWeight + bottomRight3 * bottomWeight * rightWeight + bottomLeft3 * bottomWeight * leftWeight);
        }
    }
    
    void Sampler2::Sample(Texture2& texture, Vector2& position, Fragment& fragment, Vector4& result)
    {
        auto components = texture.Components();
        
        if (components != 4)
        {
            throw runtime_error("Components must be 4.");
        }
        
        Filter filter;
        
        size_t level;
        
        FilterAndLevelFor(texture, position, fragment, filter, level);
        
        auto& buffer = texture.Buffer();
        
        auto width = texture.Width(level);
        auto height = texture.Height(level);
        auto offset = texture.Offset(level);
        
        auto widthAsFloatingPoint = (double)width;
        auto heightAsFloatingPoint = (double)height;
        
        auto s = position.X();
        auto t = position.Y();
        
        if (filter == NearestNeighbor)
        {
            double x;
            double y;
            
            bool xOutOfBounds;
            bool yOutOfBounds;
            bool xInverted;
            bool yInverted;
            
            CoordinatesFor(s, t, x, y, xOutOfBounds, yOutOfBounds, xInverted, yInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            if ((horizontalWrap == ClampToBorder && xOutOfBounds) || (verticalWrap == ClampToBorder && yOutOfBounds))
            {
                result = border;
            }
            else
            {
                size_t position = offset + ((size_t)floor(y) * width + (size_t)floor(x)) * components;
                
                result.Set(buffer[position], buffer[position + 1], buffer[position + 2], buffer[position + 3]);
            }
        }
        else
        {
            auto halfHorizontal = 1.0 / (widthAsFloatingPoint * 2);
            auto halfVertical = 1.0 / (heightAsFloatingPoint * 2);
            
            auto topLeftS = s - halfHorizontal;
            auto topLeftT = t - halfVertical;
            
            double topLeftX;
            double topLeftY;
            
            bool topLeftXOutOfBounds;
            bool topLeftYOutOfBounds;
            bool topLeftXInverted;
            bool topLeftYInverted;
            
            CoordinatesFor(topLeftS, topLeftT, topLeftX, topLeftY, topLeftXOutOfBounds, topLeftYOutOfBounds, topLeftXInverted, topLeftYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            auto bottomRightS = s + halfHorizontal;
            auto bottomRightT = t + halfVertical;
            
            double bottomRightX;
            double bottomRightY;
            
            bool bottomRightXOutOfBounds;
            bool bottomRightYOutOfBounds;
            bool bottomRightXInverted;
            bool bottomRightYInverted;
            
            CoordinatesFor(bottomRightS, bottomRightT, bottomRightX, bottomRightY, bottomRightXOutOfBounds, bottomRightYOutOfBounds, bottomRightXInverted, bottomRightYInverted, widthAsFloatingPoint, heightAsFloatingPoint);
            
            double topLeft1;
            double topLeft2;
            double topLeft3;
            double topLeft4;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topLeft1 = border.X();
                topLeft2 = border.Y();
                topLeft3 = border.Z();
                topLeft4 = border.W();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(topLeftX)) * components;
                
                topLeft1 = buffer[position];
                topLeft2 = buffer[position + 1];
                topLeft3 = buffer[position + 2];
                topLeft4 = buffer[position + 3];
            }
            
            double topRight1;
            double topRight2;
            double topRight3;
            double topRight4;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && topLeftYOutOfBounds))
            {
                topRight1 = border.X();
                topRight2 = border.Y();
                topRight3 = border.Z();
                topRight4 = border.W();
            }
            else
            {
                size_t position = offset + ((size_t)floor(topLeftY) * width + (size_t)floor(bottomRightX)) * components;
                
                topRight1 = buffer[position];
                topRight2 = buffer[position + 1];
                topRight3 = buffer[position + 2];
                topRight4 = buffer[position + 3];
            }
            
            double bottomRight1;
            double bottomRight2;
            double bottomRight3;
            double bottomRight4;
            
            if ((horizontalWrap == ClampToBorder && bottomRightXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomRight1 = border.X();
                bottomRight2 = border.Y();
                bottomRight3 = border.Z();
                bottomRight4 = border.W();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(bottomRightX)) * components;
                
                bottomRight1 = buffer[position];
                bottomRight2 = buffer[position + 1];
                bottomRight3 = buffer[position + 2];
                bottomRight4 = buffer[position + 3];
            }
            
            double bottomLeft1;
            double bottomLeft2;
            double bottomLeft3;
            double bottomLeft4;
            
            if ((horizontalWrap == ClampToBorder && topLeftXOutOfBounds) || (verticalWrap == ClampToBorder && bottomRightYOutOfBounds))
            {
                bottomLeft1 = border.X();
                bottomLeft2 = border.Y();
                bottomLeft3 = border.Z();
                bottomLeft4 = border.W();
            }
            else
            {
                size_t position = offset + ((size_t)floor(bottomRightY) * width + (size_t)floor(topLeftX)) * components;
                
                bottomLeft1 = buffer[position];
                bottomLeft2 = buffer[position + 1];
                bottomLeft3 = buffer[position + 2];
                bottomLeft4 = buffer[position + 3];
            }

            double leftWeight;
            
            if (topLeftXInverted)
            {
                leftWeight = topLeftX - floor(topLeftX);
            }
            else
            {
                leftWeight = 1 - topLeftX + floor(topLeftX);
            }

            double topWeight;
            
            if (topLeftYInverted)
            {
                topWeight = topLeftY - floor(topLeftY);
            }
            else
            {
                topWeight = 1 - topLeftY + floor(topLeftY);
            }
            
            double rightWeight;

            if (bottomRightXInverted)
            {
                rightWeight = 1 - bottomRightX + floor(bottomRightX);
            }
            else
            {
                rightWeight = bottomRightX - floor(bottomRightX);
            }
            
            double bottomWeight;
            
            if (bottomRightYInverted)
            {
                bottomWeight = 1 - bottomRightY + floor(bottomRightY);
            }
            else
            {
                bottomWeight = bottomRightY - floor(bottomRightY);
            }
            
            result.Set(topLeft1 * leftWeight * topWeight + topRight1 * rightWeight * topWeight + bottomRight1 * bottomWeight * rightWeight + bottomLeft1 * bottomWeight * leftWeight, topLeft2 * leftWeight * topWeight + topRight2 * rightWeight * topWeight + bottomRight2 * bottomWeight * rightWeight + bottomLeft2 * bottomWeight * leftWeight, topLeft3 * leftWeight * topWeight + topRight3 * rightWeight * topWeight + bottomRight3 * bottomWeight * rightWeight + bottomLeft3 * bottomWeight * leftWeight, topLeft4 * leftWeight * topWeight + topRight4 * rightWeight * topWeight + bottomRight4 * bottomWeight * rightWeight + bottomLeft4 * bottomWeight * leftWeight);
        }
    }
}
