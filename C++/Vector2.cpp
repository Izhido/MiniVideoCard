//
//  Vector2.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Vector2.h"
#include <cmath>

namespace MiniVideoCard
{
    using namespace std;

    Vector2::Vector2()
    {
        source = new vector<double>(2);
        ownsSource = true;
        offset = 0;
    }

    Vector2::Vector2(double x, double y) : Vector2()
    {
        Set(x, y);
    }
    
    Vector2::Vector2(vector<double>* source, size_t offset)
    {
        if (source == nullptr)
        {
            throw runtime_error("Source cannot be null.");
        }
        
        if (offset + 2 > source->size())
        {
            throw runtime_error("Offset exceeds available source size.");
        }
        
        this->source = source;
        ownsSource = false;
        this->offset = offset;
    }
    
    Vector2::Vector2(const Vector2& source) : Vector2()
    {
        Set(source);
    }

    Vector2::Vector2(Vector2&& source)
    {
        if (source.ownsSource)
        {
            this->source = source.source;
            ownsSource = source.ownsSource;
            offset = source.offset;
            
            source.ownsSource = false;
        }
        else
        {
            Set(source);
        }
    }

	vector<double>* Vector2::Source()
	{
		return source;
	}

	size_t Vector2::Offset()
	{
		return offset;
	}

    double Vector2::X() const
    {
        return source->at(offset);
    }
    
    void Vector2::SetX(double x)
    {
        source->at(offset) = x;
    }
    
    double Vector2::Y() const
    {
        return source->at(offset + 1);
    }
    
    void Vector2::SetY(double y)
    {
        source->at(offset + 1) = y;
    }
    
    void Vector2::Set(double x, double y)
    {
        SetX(x);
        SetY(y);
    }

    void Vector2::Add(const Vector2& right, Vector2& result) const
    {
        result.Set(X() + right.X(), Y() + right.Y());
    }
    
    void Vector2::Subtract(const Vector2& right, Vector2& result) const
    {
        result.Set(X() - right.X(), Y() - right.Y());
    }
    
    void Vector2::Multiply(double right, Vector2 &result) const
    {
        result.Set(X() * right, Y() * right);
    }
    
    void Vector2::Divide(double right, Vector2 &result) const
    {
        result.Set(X() / right, Y() / right);
    }
    
    double Vector2::Dot(const Vector2& right) const
    {
        return X() * right.X() + Y() * right.Y();
    }
    
    double Vector2::SquareLength()
    {
        return Dot(*this);
    }
    
    double Vector2::Length()
    {
        return sqrt(SquareLength());
    }
    
    bool Vector2::Normalize()
    {
        auto length = Length();
        
        if (length == 0)
        {
            return false;
        }
        
        Set(X() / length, Y() / length);
        
        return true;
    }
    
    void Vector2::Set(const Vector2& source)
    {
        *(this->source) = *(source.source);
    }

    Vector2& Vector2::operator=(const Vector2& right)
    {
        Set(right);
        
        return *this;
    }

    Vector2::~Vector2()
    {
        if (ownsSource)
        {
            delete source;
        }
    }
    
    Vector2 operator+(const Vector2& left, const Vector2& right)
    {
        Vector2 result;
        
        left.Add(right, result);
        
        return result;
    }
    
    Vector2 operator-(const Vector2& left, const Vector2& right)
    {
        Vector2 result;
        
        left.Subtract(right, result);
        
        return result;
    }
    
    Vector2 operator*(const Vector2& left, double right)
    {
        Vector2 result;
        
        left.Multiply(right, result);
        
        return result;
    }
    
    Vector2 operator*(double left, const Vector2& right)
    {
        Vector2 result;
        
        right.Multiply(left, result);
        
        return result;
    }
    
    double operator*(const Vector2& left, const Vector2& right)
    {
        return left.Dot(right);
    }
    
    Vector2 operator/(const Vector2& left, double right)
    {
        Vector2 result;
        
        left.Divide(right, result);
        
        return result;
    }
}
