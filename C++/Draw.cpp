//
//  Draw.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Draw.h"
#include "Data.h"

namespace MiniVideoCard
{
    using namespace std;
    
    Draw::Draw(bool deleteDataAfterUse, vector<class Data*>* data)
    {
        this->data = data;
        this->deleteDataAfterUse = deleteDataAfterUse;
        this->indices = nullptr;
        this->deleteIndicesAfterUse = false;
        this->offset = 0;

        if (data != nullptr)
        {
            this->count = data->size();
        }
    }
    
    Draw::Draw(bool deleteDataAfterUse, vector<class Data*>* data, size_t offset, size_t count)
    {
        this->data = data;
        this->deleteDataAfterUse = deleteDataAfterUse;
        this->indices = nullptr;
        this->deleteIndicesAfterUse = false;
        this->offset = offset;
        this->count = count;
    }
    
    Draw::Draw(bool deleteDataAfterUse, vector<class Data*>* data, bool deleteIndicesAfterUse, vector<size_t>* indices)
    {
        this->data = data;
        this->deleteDataAfterUse = deleteDataAfterUse;
        this->indices = indices;
        this->deleteIndicesAfterUse = deleteIndicesAfterUse;
        this->offset = 0;
        
        if (indices != nullptr)
        {
            this->count = indices->size();
        }
    }
    
    Draw::Draw(bool deleteDataAfterUse, vector<class Data*>* data, bool deleteIndicesAfterUse, vector<size_t>* indices, size_t offset, size_t count)
    {
        this->data = data;
        this->deleteDataAfterUse = deleteDataAfterUse;
        this->indices = indices;
        this->deleteIndicesAfterUse = deleteIndicesAfterUse;
        this->offset = offset;
        this->count = count;
    }
    
    vector<Data*>* Draw::Data()
    {
        return data;
    }
    
    vector<size_t>* Draw::Indices()
    {
        return indices;
    }
    
    size_t Draw::Offset()
    {
        return offset;
    }
    
    size_t Draw::Count()
    {
        return count;
    }
    
    Draw::~Draw()
    {
        if (deleteDataAfterUse && data != nullptr)
        {
            for (auto parameter = data->begin(); parameter != data->end(); parameter++)
            {
                delete *parameter;
            }
            
            delete data;
        }
        
        if (deleteIndicesAfterUse)
        {
            delete indices;
        }
    }
}
