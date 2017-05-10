//
//  Draw.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include "Command.h"
#include <vector>

namespace MiniVideoCard
{
    using namespace std;
    
    class Data;
    
    class Draw : public Command
    {
    public:
        Draw(bool deleteDataAfterUse, vector<Data*>* data);
        
        Draw(bool deleteDataAfterUse, vector<Data*>* data, size_t offset, size_t count);
        
        Draw(bool deleteDataAfterUse, vector<Data*>* data, bool deleteIndicesAfterUse, vector<size_t>* indices);

        Draw(bool deleteDataAfterUse, vector<Data*>* data, bool deleteIndicesAfterUse, vector<size_t>* indices, size_t offset, size_t count);
        
        vector<Data*>* Data();
        
        vector<size_t>* Indices();

        size_t Offset();
        
        size_t Count();
        
        ~Draw();
        
    private:
        vector<class Data*>* data;
        
        bool deleteDataAfterUse;
        
        vector<size_t>* indices;
        
        bool deleteIndicesAfterUse;
        
        size_t offset;
        
        size_t count;
    };
}
