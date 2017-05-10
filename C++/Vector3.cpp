//
//  Vector3.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Vector3.h"
#include <cmath>

namespace MiniVideoCard
{
    using namespace std;
    
    Vector3::Vector3()
    {
        source = new vector<double>(3);
        ownsSource = true;
        offset = 0;
    }

    Vector3::Vector3(double x, double y, double z) : Vector3()
    {
        Set(x, y, z);
    }
    
    Vector3::Vector3(vector<double>* source, size_t offset)
    {
        if (source == nullptr)
        {
            throw runtime_error("Source cannot be null.");
        }
        
        if (offset + 3 > source->size())
        {
            throw runtime_error("Offset exceeds available source size.");
        }
        
        this->source = source;
        ownsSource = false;
        this->offset = offset;
    }
    
    Vector3::Vector3(const Vector3& source) : Vector3()
    {
        Set(source);
    }
    
    Vector3::Vector3(Vector3&& source)
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
    
    Vector3::Vector3(Vector2& source, double z) : Vector3()
    {
        Set(source, z);
    }
    
	vector<double>* Vector3::Source()
	{
		return source;
	}

	size_t Vector3::Offset()
	{
		return offset;
	}

	double Vector3::X() const
    {
        return source->at(offset);
    }
    
    void Vector3::SetX(double x)
    {
        source->at(offset) = x;
    }
    
    double Vector3::Y() const
    {
        return source->at(offset + 1);
    }
    
    void Vector3::SetY(double y)
    {
        source->at(offset + 1) = y;
    }
    
    double Vector3::Z() const
    {
        return source->at(offset + 2);
    }
    
    void Vector3::SetZ(double z)
    {
        source->at(offset + 2) = z;
    }
    
    void Vector3::Set(double x, double y, double z)
    {
        SetX(x);
        SetY(y);
        SetZ(z);
    }
    
    void Vector3::Set(const Vector3& source)
    {
        *(this->source) = *(source.source);
    }
    
    void Vector3::Set(Vector2& source, double z)
    {
        Set(source.X(), source.Y(), z);
    }
    
	Vector2 Vector3::XY()
	{
		return Vector2(X(), Y());
	}

    void Vector3::Add(const Vector3& right, Vector3& result) const
    {
        result.Set(X() + right.X(), Y() + right.Y(), Z() + right.Z());
    }
    
    void Vector3::Subtract(const Vector3& right, Vector3& result) const
    {
        result.Set(X() - right.X(), Y() - right.Y(), Z() - right.Z());
    }
    
    void Vector3::Multiply(double right, Vector3 &result) const
    {
        result.Set(X() * right, Y() * right, Z() * right);
    }
    
    void Vector3::Divide(double right, Vector3 &result) const
    {
        result.Set(X() / right, Y() / right, Z() / right);
    }
    
    double Vector3::Dot(const Vector3& right) const
    {
        return X() * right.X() + Y() * right.Y() + Z() * right.Z();
    }
    
    void Vector3::Cross(const Vector3& right, Vector3& result) const
    {
        result.Set(Y() * right.Z() - Z() * right.Y(), Z() * right.X() - X() * right.Z(), X() * right.Y() - Y() * right.X());
    }
    
    double Vector3::SquareLength()
    {
        return Dot(*this);
    }
    
    double Vector3::Length()
    {
        return sqrt(SquareLength());
    }
    
    bool Vector3::Normalize()
    {
        auto length = Length();
        
        if (length == 0)
        {
            return false;
        }
        
        Set(X() / length, Y() / length, Z() / length);
        
        return true;
    }

    Vector3& Vector3::operator=(const Vector3& right)
    {
        Set(right);
        
        return *this;
    }

	Vector3::~Vector3()
    {
        if (ownsSource)
        {
            delete source;
        }
    }

    Vector3 operator+(const Vector3& left, const Vector3& right)
    {
        Vector3 result;
        
        left.Add(right, result);
        
        return result;
    }
    
    Vector3 operator-(const Vector3& left, const Vector3& right)
    {
        Vector3 result;
        
        left.Subtract(right, result);
        
        return result;
    }
    
    Vector3 operator*(const Vector3& left, double right)
    {
        Vector3 result;
        
        left.Multiply(right, result);
        
        return result;
    }
    
    Vector3 operator*(double left, const Vector3& right)
    {
        Vector3 result;
        
        right.Multiply(left, result);
        
        return result;
    }
    
    double operator*(const Vector3& left, const Vector3& right)
    {
        return left.Dot(right);
    }
    
    Vector3 operator^(const Vector3& left, const Vector3& right)
    {
        Vector3 result;
        
        left.Cross(right, result);
        
        return result;
    }

    Vector3 operator/(const Vector3& left, double right)
    {
        Vector3 result;
        
        left.Divide(right, result);
        
        return result;
    }
}
