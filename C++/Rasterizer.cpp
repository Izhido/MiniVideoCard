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
        
        auto xw1 = floor(((double)width / 2) * xd1 + ((double)width / 2));
        auto yw1 = floor(((double)height / 2) * yd1 + ((double)height / 2));
        auto zw1 = 0.5 * zd1 + 0.5;

        auto xw2 = floor(((double)width / 2) * xd2 + ((double)width / 2));
        auto yw2 = floor(((double)height / 2) * yd2 + ((double)height / 2));
        auto zw2 = 0.5 * zd2 + 0.5;
        
        auto xw3 = floor(((double)width / 2) * xd3 + ((double)width / 2));
        auto yw3 = floor(((double)height / 2) * yd3 + ((double)height / 2));
        auto zw3 = 0.5 * zd3 + 0.5;
        
        auto area = fabs(xw1 * yw2 + xw2 * yw3 + xw3 * yw1 - xw2 * yw1 - xw3 * yw2 - xw1 * yw3) / 2;
        
        if (area == 0)
        {
            return;
        }
        
        auto left = min(min(xw1, xw2), xw3);
        auto bottom = min(min(yw1, yw2), yw3);
        auto right = max(max(xw1, xw2), xw3);
        auto top = max(max(yw1, yw2), yw3);
        
        size_t scanWidth = 0;
        
        size_t v = 0;
        
        for (auto y = bottom + 0.5; y < top + 1; y++)
        {
            size_t h = 0;
            
            for (auto x = left + 0.5; x < right + 1; x++)
            {
                auto area12 = fabs(x * yw1 + xw1 * yw2 + xw2 * y - xw1 * y - xw2 * yw1 - x * yw2) / 2;
                auto area13 = fabs(x * yw1 + xw1 * yw3 + xw3 * y - xw1 * y - xw3 * yw1 - x * yw3) / 2;
                auto area23 = fabs(x * yw2 + xw2 * yw3 + xw3 * y - xw2 * y - xw3 * yw2 - x * yw3) / 2;

                auto a = area23 / area;
                auto b = area13 / area;
                auto c = area12 / area;
                
                auto z = a * zw1 + b * zw2 + c * zw3;
                
                fragments.emplace_back(x, y, z, count1);
                
                auto& fragmentVaryings = fragments.back().Varyings();

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
                
				size_t position = v * scanWidth + h;
				
				if (h > 0)
                {
                    fragments[position - 1].SetVaryingsAtRight(&fragmentVaryings);
                }

				if (v > 0)
				{
					fragments[position - scanWidth].SetVaryingsBelow(&fragmentVaryings);
				}
                
                h++;
            }
            
            if (v == 0)
            {
                scanWidth = h;
            }
            
            v++;
        }

        size_t position = 0;
        
        for (auto y = bottom + 0.5; y < top + 1; y++)
        {
            for (auto x = left + 0.5; x < right + 1; x++)
            {
                Fragment& fragment = fragments[position];
                
                position++;
                
                if (y >= 0 && y < (double)height && x >= 0 && x < (double)width)
                {
                    auto sign0 = (x - xw2) * (yw1 - yw2) - (xw1 - xw2) * (y - yw2);
                    auto sign1 = (x - xw3) * (yw2 - yw3) - (xw2 - xw3) * (y - yw3);
                    auto sign2 = (x - xw1) * (yw3 - yw1) - (xw3 - xw1) * (y - yw1);
                    
                    if (((sign0 < 0 && sign1 < 0) || (sign0 >= 0 && sign1 >= 0)) && ((sign1 < 0 && sign2 < 0) || (sign1 >= 0 && sign2 >= 0)))
                    {
                        auto inBuffer = (height - 1 - (size_t)floor(fragment.Y())) * width + (size_t)floor(fragment.X());
                        
                        auto depth = fragment.Z();
                        
                        if (depth >= 0 && depth <= 1 && depth < depthBuffer[inBuffer])
                        {
                            render.push_back(true);
                            
                            continue;
                        }
                    }
                }
                
                render.push_back(false);
            }
        }
        
        for (size_t i = 0; i < fragments.size(); i++)
        {
            if (render[i])
            {
                Fragment& fragment = fragments[i];
                
                fragmentShader->Run(&fragment, parameters);
                
                if (fragment.Discarded())
                {
                    render[i] = false;
                }
            }
        }
        
        for (size_t i = 0; i < fragments.size(); i++)
        {
            if (render[i])
            {
                Fragment& fragment = fragments[i];
                
                auto x = (size_t)floor(fragment.X());
                auto y = (size_t)floor(fragment.Y());
                auto z = fragment.Z();
                
                auto positionDepth = (height - 1 - y) * width + x;
                
                depthBuffer[positionDepth] = z;

                auto positionR = positionDepth * 4;
                auto positionG = positionR + 1;
                auto positionB = positionG + 1;
                auto positionA = positionB + 1;
                
                auto color = fragment.Color();
                
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
        }
        
        fragments.clear();
        
        render.clear();
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
