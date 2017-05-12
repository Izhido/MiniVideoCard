//
//  Rasterizer.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Rasterizer.h"
#include "ClearColorBuffer.h"
#include "ClearDepthBuffer.h"
#include "UseVertexShader.h"
#include "UseFragmentShader.h"
#include "SetParameters.h"
#include "Draw.h"
#include "VertexShader.h"
#include "Vertex.h"
#include "Fragment.h"
#include "FragmentShader.h"
#include <cmath>
#include <algorithm>
#include <typeinfo>

namespace MiniVideoCard
{
    using namespace std;
    
    Rasterizer::Rasterizer(size_t width, size_t height)
    {
        if (width <= 0)
        {
            throw new runtime_error("Width must be greater than zero.");
        }

        if (height <= 0)
        {
            throw new runtime_error("Height must be greater than zero.");
        }
        
        this->width = width;
        
        this->height = height;
        
        auto size = width * height;
        
        colorBuffer.resize(size * 4);
        
        depthBuffer.resize(size);
        
        vertexShader = nullptr;
        
        fragmentShader = nullptr;
        
        parameters = nullptr;
    }
        
    size_t Rasterizer::Width()
    {
        return width;
    }
        
    size_t Rasterizer::Height()
    {
        return height;
    }
        
    vector<double>& Rasterizer::ColorBuffer()
    {
        return colorBuffer;
    }
        
    vector<double>& Rasterizer::DepthBuffer()
    {
        return depthBuffer;
    }
    
    void Rasterizer::AddCommand(bool deleteAfterUse, Command* command)
    {
        commands.push_back(command);
        toDelete.push_back(deleteAfterUse);
    }
    
    void Rasterizer::Run()
    {
        for (auto entry = commands.begin(); entry != commands.end(); entry++)
        {
            auto command = *entry;
            
            if (typeid(*command) == typeid(ClearColorBuffer))
            {
                auto clearColorBufferCommand = (ClearColorBuffer *)command;
                
                auto r = clearColorBufferCommand->R();
                auto g = clearColorBufferCommand->G();
                auto b = clearColorBufferCommand->B();
                auto a = clearColorBufferCommand->A();

                for (size_t i = 0; i < colorBuffer.size(); i += 4)
                {
                    colorBuffer[i] = r;
                    colorBuffer[i + 1] = g;
                    colorBuffer[i + 2] = b;
                    colorBuffer[i + 3] = a;
                }
            }
            else if (typeid(*command) == typeid(ClearDepthBuffer))
            {
                auto clearDepthBufferCommand = (ClearDepthBuffer *)command;
                
                auto depth = clearDepthBufferCommand->Depth();
                
                for (size_t i = 0; i < depthBuffer.size(); i++)
                {
                    depthBuffer[i] = depth;
                }
            }
            else if (typeid(*command) == typeid(UseVertexShader))
            {
                auto useVertexShaderCommand = (UseVertexShader *)command;
                
                vertexShader = useVertexShaderCommand->VertexShader();
            }
            else if (typeid(*command) == typeid(UseFragmentShader))
            {
                auto useFragmentShaderCommand = (UseFragmentShader *)command;
                
                fragmentShader = useFragmentShaderCommand->FragmentShader();
            }
            else if (typeid(*command) == typeid(SetParameters))
            {
                auto setParametersCommand = (SetParameters *)command;
                
                parameters = setParametersCommand->Parameters();
            }
            else if (typeid(*command) == typeid(Draw))
            {
                auto drawCommand = (Draw *)command;
                
                auto vertices = drawCommand->Data();
                
                if (vertices != nullptr)
                {
                    auto offset = drawCommand->Offset();
                    
                    auto count =  drawCommand->Count();
                    
                    auto indices = drawCommand->Indices();
                    
                    if (indices != nullptr)
                    {
                        auto begin = min(offset, indices->size() - 1);
                        
                        auto end = min(offset + count, indices->size() - 1);
                        
                        auto position = 0;
                        
                        auto isValid = true;
                        
                        Data* triangle[3];
                        
                        while (begin <= end)
                        {
                            if (isValid)
                            {
                                auto parameterIndex = indices->at(begin);
                                
                                if (parameterIndex >= vertices->size())
                                {
                                    isValid = false;
                                }
                                else
                                {
                                    triangle[position] = vertices->at(parameterIndex);
                                }
                            }
                            
                            position++;
                            
                            if (position == 3)
                            {
                                if (isValid)
                                {
                                    DrawTriangle(triangle);
                                }
                                
                                position = 0;
                                
                                isValid = true;
                            }
                            
                            begin++;
                        }
                    }
                    else
                    {
                        auto begin = min(offset, vertices->size() - 1);
                        
                        auto end = min(offset + count, vertices->size() - 1);
                        
                        auto position = 0;
                        
                        Data* triangle[3];
                        
                        while (begin <= end)
                        {
                            triangle[position] = vertices->at(begin);
                            
                            position++;
                            
                            if (position == 3)
                            {
                                DrawTriangle(triangle);
                                
                                position = 0;
                            }
                            
                            begin++;
                        }
                    }
                }
            }
        }
        
        ClearCommands();
    }
    
    size_t Rasterizer::PositionFor(double x, double y)
    {
        return (height - 1 - (size_t)floor(y)) * width + (size_t)floor(x);
    }

    void Rasterizer::DrawTriangle(Data* vertices[3])
    {
        Vertex vertex1;
        Vertex vertex2;
        Vertex vertex3;
        
        vertexShader->Run(vertices[0], &vertex1, parameters);
        
        vertexShader->Run(vertices[1], &vertex2, parameters);
        
        vertexShader->Run(vertices[2], &vertex3, parameters);
        
        auto& vertex1Varyings = vertex1.Varyings();
        auto& vertex2Varyings = vertex2.Varyings();
        auto& vertex3Varyings = vertex3.Varyings();
        
        auto count1 = vertex1Varyings.size();
        auto count2 = vertex2Varyings.size();
        auto count3 = vertex3Varyings.size();
        
        if (count1 != count2 || count2 != count3 || count3 != count1)
        {
            return;
        }
        
        auto xc1 = vertex1.Position().X();
        auto yc1 = vertex1.Position().Y();
        auto zc1 = vertex1.Position().Z();
        auto wc1 = vertex1.Position().W();
        
        auto xc2 = vertex2.Position().X();
        auto yc2 = vertex2.Position().Y();
        auto zc2 = vertex2.Position().Z();
        auto wc2 = vertex2.Position().W();

        auto xc3 = vertex3.Position().X();
        auto yc3 = vertex3.Position().Y();
        auto zc3 = vertex3.Position().Z();
        auto wc3 = vertex3.Position().W();

        if (wc1 == 0 || wc2 == 0 || wc3 == 0)
        {
            return;
        }

        auto xd1 = xc1 / wc1;
        auto yd1 = yc1 / wc1;
        auto zd1 = zc1 / wc1;
        
        auto xd2 = xc2 / wc2;
        auto yd2 = yc2 / wc2;
        auto zd2 = zc2 / wc2;
        
        auto xd3 = xc3 / wc3;
        auto yd3 = yc3 / wc3;
        auto zd3 = zc3 / wc3;
        
        if ((xd1 < -1 && xd2 < -1 && xd3 < -1) || (yd1 < -1 && yd2 < -1 && yd3 < -1) || (zd1 < -1 && zd2 < -1 && zd3 < -1) || (xd1 >= 1 && xd2 >= 1 && xd3 >= 1) || (yd1 >= 1 && yd2 >= 1 && yd3 >= 1) || (zd1 >= 1 && zd2 >= 1 && zd3 >= 1))
        {
            return;
        }
        
        auto widthAsDouble = (double)width;
        auto heightAsDouble = (double)height;
        
        auto halfWidthAsDouble = widthAsDouble / 2;
        auto halfHeightAsDouble = heightAsDouble / 2;
        
        auto xw1 = floor(halfWidthAsDouble * xd1 + halfWidthAsDouble);
        auto yw1 = floor(halfHeightAsDouble * yd1 + halfHeightAsDouble);
        auto zw1 = 0.5 * zd1 + 0.5;

        auto xw2 = floor(halfWidthAsDouble * xd2 + halfWidthAsDouble);
        auto yw2 = floor(halfHeightAsDouble * yd2 + halfHeightAsDouble);
        auto zw2 = 0.5 * zd2 + 0.5;
        
        auto xw3 = floor(halfWidthAsDouble * xd3 + halfWidthAsDouble);
        auto yw3 = floor(halfHeightAsDouble * yd3 + halfHeightAsDouble);
        auto zw3 = 0.5 * zd3 + 0.5;
        
        auto xw1TimesY2 = xw1 * yw2;
        auto xw2TimesY1 = xw2 * yw1;
        auto xw1TimesY3 = xw1 * yw3;
        auto xw3TimesY1 = xw3 * yw1;
        auto xw2TimesY3 = xw2 * yw3;
        auto xw3TimesY2 = xw3 * yw2;

        auto xw1Minus2 = xw1 - xw2;
        auto yw1Minus2 = yw1 - yw2;
        auto xw2Minus3 = xw2 - xw3;
        auto yw2Minus3 = yw2 - yw3;
        auto xw3Minus1 = xw3 - xw1;
        auto yw3Minus1 = yw3 - yw1;
        
        auto areaTimes2 = fabs(xw1 * yw2 + xw2 * yw3 + xw3 * yw1 - xw2 * yw1 - xw3 * yw2 - xw1 * yw3);
        
        if (areaTimes2 == 0)
        {
            return;
        }
        
        auto left = max(min(min(xw1, xw2), xw3), 0.0);
        auto bottom = max(min(min(yw1, yw2), yw3), 0.0);
        auto right = min(max(max(xw1, xw2), xw3), widthAsDouble - 1);
        auto top = min(max(max(yw1, yw2), yw3), heightAsDouble - 1);
        
        size_t scanWidth = 0;
        
        size_t v = 0;
        
        for (auto y = bottom + 0.5; y < top + 1; y += 1)
        {
            size_t h = 0;
            
            for (auto x = left + 0.5; x < right + 1; x += 1)
            {
                auto area12Times2 = fabs(x * yw1 + xw1TimesY2 + xw2 * y - xw1 * y - xw2TimesY1 - x * yw2);
                auto area13Times2 = fabs(x * yw1 + xw1TimesY3 + xw3 * y - xw1 * y - xw3TimesY1 - x * yw3);
                auto area23Times2 = fabs(x * yw2 + xw2TimesY3 + xw3 * y - xw2 * y - xw3TimesY2 - x * yw3);

                auto a = area23Times2 / areaTimes2;
                auto b = area13Times2 / areaTimes2;
                auto c = area12Times2 / areaTimes2;
                
                auto z = a * zw1 + b * zw2 + c * zw3;
                
                bool discarded = true;
                
                if (x >= 0 && x < widthAsDouble && y >= 0 && y < heightAsDouble)
                {
                    auto sign0 = (x - xw2) * yw1Minus2 - xw1Minus2 * (y - yw2);
                    auto sign1 = (x - xw3) * yw2Minus3 - xw2Minus3 * (y - yw3);
                    auto sign2 = (x - xw1) * yw3Minus1 - xw3Minus1 * (y - yw1);
                    
                    if (((sign0 < 0 && sign1 < 0) || (sign0 >= 0 && sign1 >= 0)) && ((sign1 < 0 && sign2 < 0) || (sign1 >= 0 && sign2 >= 0)))
                    {
                        auto position = PositionFor(x, y);
                        
                        if (z >= 0 && z <= 1 && z < depthBuffer[position])
                        {
                            discarded = false;
                        }
                    }
                }
                
                fragments.push_back(new Fragment(x, y, z, count1, discarded));
                
                auto& fragmentVaryings = fragments.back()->Varyings();

                if (count1 > 0)
                {
                    for (size_t i = 0; i < count1; i++)
                    {
                        auto fa = vertex1Varyings[i];
                        auto fb = vertex2Varyings[i];
                        auto fc = vertex3Varyings[i];
                        
                        fragmentVaryings[i] = ((a * fa / wc1) + (b * fb / wc2) + (c * fc / wc3)) / (a / wc1 + b / wc2 + c / wc3);
                    }
                }
                
				size_t p = v * scanWidth + h;
				
				if (h > 0)
                {
                    fragments[p - 1]->SetVaryingsAtRight(&fragmentVaryings);
                }

				if (v > 0)
				{
					fragments[p - scanWidth]->SetVaryingsBelow(&fragmentVaryings);
				}
                
                h++;
            }
            
            if (v == 0)
            {
                scanWidth = h;
            }
            
            v++;
        }

        for (size_t i = 0; i < fragments.size(); i++)
        {
            auto fragment = fragments[i];

            if (fragment->Discarded())
            {
                continue;
            }

            fragmentShader->Run(fragment, parameters);
        }
    
        for (size_t i = 0; i < fragments.size(); i++)
        {
            auto fragment = fragments[i];
            
            if (fragment->Discarded())
            {
                continue;
            }
            
            auto& position = fragment->Position();
            
            auto positionDepth = PositionFor(position.X(), position.Y());
            
            auto depth = position.Z();
            
            depthBuffer[positionDepth] = depth;
            
            auto positionR = positionDepth * 4;
            auto positionG = positionR + 1;
            auto positionB = positionG + 1;
            auto positionA = positionB + 1;
            
            auto color = fragment->Color();
            
            auto sourceR = color.X();
            auto sourceG = color.Y();
            auto sourceB = color.Z();
            auto sourceA = color.W();
            
            auto destinationR = colorBuffer[positionR];
            auto destinationG = colorBuffer[positionG];
            auto destinationB = colorBuffer[positionB];
            auto destinationA = colorBuffer[positionA];
            
            colorBuffer[positionR] = sourceR * sourceA + destinationR * (1 - sourceA);
            colorBuffer[positionG] = sourceG * sourceA + destinationG * (1 - sourceA);
            colorBuffer[positionB] = sourceB * sourceA + destinationB * (1 - sourceA);
            colorBuffer[positionA] = sourceA * sourceA + destinationA * (1 - sourceA);
        }
    
        for (size_t i = 0; i < fragments.size(); i++)
        {
            delete fragments[i];
        }
        
        fragments.clear();
    }

    void Rasterizer::ClearCommands()
    {
        for (size_t i = 0; i < toDelete.size(); i++)
        {
            if (toDelete[i])
            {
                delete commands[i];
            }
        }
        
        commands.clear();
        toDelete.clear();
    }
    
    Rasterizer::~Rasterizer()
    {
        ClearCommands();
    }
}
