//
//  Vector4.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Vector4.h"
#include <cmath>

namespace MiniVideoCard
{
    using namespace std;
    
    Vector4::Vector4()
    {
        source = new vector<double>(4);
        ownsSource = true;
        offset = 0;
    }

    Vector4::Vector4(double x, double y, double z) : Vector4()
    {
        Set(x, y, z);
    }
    
    Vector4::Vector4(double x, double y, double z, double w) : Vector4()
    {
        Set(x, y, z, w);
    }
    
    Vector4::Vector4(vector<double>* source, size_t offset)
    {
        if (source == nullptr)
        {
            throw runtime_error("Source cannot be null.");
        }
        
        if (offset + 4 > source->size())
        {
            throw runtime_error("Offset exceeds available source size.");
        }
        
        this->source = source;
        ownsSource = false;
        this->offset = offset;
    }

    Vector4::Vector4(const Vector4& source) : Vector4()
    {
        Set(source);
    }
    
    Vector4::Vector4(Vector4&& source)
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
    
    Vector4::Vector4(Vector3& source) : Vector4()
    {
        Set(source);
    }
    
    Vector4::Vector4(Vector3& source, double w) : Vector4()
    {
        Set(source, w);
    }
    
    Vector4::Vector4(Vector2& source, double z) : Vector4()
    {
        Set(source, z);
    }
    
    Vector4::Vector4(Vector2& source, double z, double w) : Vector4()
    {
        Set(source, z, w);
    }

	vector<double>* Vector4::Source()
	{
		return source;
	}

	size_t Vector4::Offset()
	{
		return offset;
	}

	double Vector4::X() const
    {
        return source->at(offset);
    }
    
    void Vector4::SetX(double x)
    {
        source->at(offset) = x;
    }

    double Vector4::Y() const
    {
        return source->at(offset + 1);
    }
    
    void Vector4::SetY(double y)
    {
        source->at(offset + 1) = y;
    }
    
    double Vector4::Z() const
    {
        return source->at(offset + 2);
    }
    
    void Vector4::SetZ(double z)
    {
        source->at(offset + 2) = z;
    }
    
    double Vector4::W() const
    {
        return source->at(offset + 3);
    }
    
    void Vector4::SetW(double w)
    {
        source->at(offset + 3) = w;
    }
    
    void Vector4::Set(double x, double y, double z)
    {
        SetX(x);
        SetY(y);
        SetZ(z);
        SetW(1);
    }
    
    void Vector4::Set(double x, double y, double z, double w)
    {
        SetX(x);
        SetY(y);
        SetZ(z);
        SetW(w);
    }
    
    void Vector4::Set(const Vector4& source)
    {
        *(this->source) = *(source.source);
    }
    
    void Vector4::Set(Vector3& source)
    {
        Set(source.X(), source.Y() ,source.Z());
    }
    
    void Vector4::Set(Vector3& source, double w)
    {
        Set(source.X(), source.Y(), source.Z(), w);
    }
    
    void Vector4::Set(Vector2& source, double z)
    {
        Set(source.X(), source.Y(), z);
    }
    
    void Vector4::Set(Vector2& source, double z, double w)
    {
        Set(source.X(), source.Y(), z, w);
    }
    
	Vector2 Vector4::XY()
	{
		return Vector2(X(), Y());
	}

	Vector3 Vector4::XYZ()
	{
		return Vector3(X(), Y(), Z());
	}

    void Vector4::Add(const Vector4& right, Vector4& result) const
    {
        result.Set(X() + right.X(), Y() + right.Y(), Z() + right.Z(), W() + right.W());
    }
    
    void Vector4::Subtract(const Vector4& right, Vector4& result) const
    {
        result.Set(X() - right.X(), Y() - right.Y(), Z() - right.Z(), W() - right.W());
    }
    
    void Vector4::Multiply(double right, Vector4 &result) const
    {
        result.Set(X() * right, Y() * right, Z() * right, W() * right);
    }

    void Vector4::Divide(double right, Vector4 &result) const
    {
        result.Set(X() / right, Y() / right, Z() / right, W() / right);
    }
    
    double Vector4::Dot(const Vector4& right) const
    {
        return X() * right.X() + Y() * right.Y() + Z() * right.Z() + W() * right.W();
    }
    
    double Vector4::SquareLength()
    {
        return Dot(*this);
    }
    
    double Vector4::Length()
    {
        return sqrt(SquareLength());
    }
    
    bool Vector4::Normalize()
    {
        auto length = Length();
        
        if (length == 0)
        {
            return false;
        }
        
        Set(X() / length, Y() / length, Z() / length, W() / length);
        
        return true;
    }

    Vector4& Vector4::operator=(const Vector4& right)
    {
        Set(right);
        
        return *this;
    }
    
    Vector4::~Vector4()
    {
        if (ownsSource)
        {
            delete source;
        }
    }
    
    Vector4 operator+(const Vector4& left, const Vector4& right)
    {
        Vector4 result;
        
        left.Add(right, result);
        
        return result;
    }
    
    Vector4 operator-(const Vector4& left, const Vector4& right)
    {
        Vector4 result;
        
        left.Subtract(right, result);
        
        return result;
    }

    Vector4 operator*(const Vector4& left, double right)
    {
        Vector4 result;
        
        left.Multiply(right, result);
        
        return result;
    }
    
    Vector4 operator*(double left, const Vector4& right)
    {
        Vector4 result;
        
        right.Multiply(left, result);
        
        return result;
    }
    
    double operator*(const Vector4& left, const Vector4& right)
    {
        return left.Dot(right);
    }
    
    Vector4 operator/(const Vector4& left, double right)
    {
        Vector4 result;
        
        left.Divide(right, result);
        
        return result;
    }
}
