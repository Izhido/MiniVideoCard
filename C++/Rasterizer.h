//
//  Rasterizer.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <cstddef>
#include <vector>

namespace MiniVideoCard
{
    using namespace std;
    
    class Command;
    class VertexShader;
    class FragmentShader;
    class Data;
    class Fragment;
    class Parameters;
    
    class Rasterizer
    {
    public:
        Rasterizer(size_t width, size_t height);
        
        size_t Width();
        
        size_t Height();
        
        vector<double>& ColorBuffer();
        
        vector<double>& DepthBuffer();
        
        void AddCommand(bool deleteAfterUse, Command* command);
        
        void Run();
        
        ~Rasterizer();
        
    private:
        size_t width;
        
        size_t height;
        
        vector<double> colorBuffer;

        vector<double> depthBuffer;
        
        vector<Command*> commands;
        
        vector<bool> toDelete;
        
        VertexShader* vertexShader;
        
        FragmentShader* fragmentShader;
        
        Parameters* parameters;
        
        vector<Fragment*> fragments;
        
        size_t PositionFor(double x, double y);
        
        void DrawTriangle(Data* vertices[3]);
        
        void ClearCommands();
        
        void ClearFragments();
    };
}
