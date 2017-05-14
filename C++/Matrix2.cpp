//
//  Matrix2.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Matrix2.h"

namespace MiniVideoCard
{
    using namespace std;
    
    Matrix2::Matrix2()
    {
        source = new vector<double>(4);
        ownsSource = true;
        offset = 0;
    }

    Matrix2::Matrix2(double m11, double m21, double m12, double m22) : Matrix2()
    {
        Set(m11, m21, m12, m22);
    }
    
    Matrix2::Matrix2(vector<double>* source, size_t offset)
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
    
    Matrix2::Matrix2(const Matrix2& source) : Matrix2()
    {
        Set(source);
    }

    Matrix2::Matrix2(Matrix2&& source)
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
    
	vector<double>* Matrix2::Source()
	{
		return source;
	}

	size_t Matrix2::Offset()
	{
		return offset;
	}

	double Matrix2::M11() const
    {
        return source->at(offset);
    }
    
    void Matrix2::SetM11(double m11)
    {
        source->at(offset) = m11;
    }
    
    double Matrix2::M21() const
    {
        return source->at(offset + 1);
    }
    
    void Matrix2::SetM21(double m21)
    {
        source->at(offset + 1) = m21;
    }
    
    double Matrix2::M12() const
    {
        return source->at(offset + 2);
    }
    
    void Matrix2::SetM12(double m12)
    {
        source->at(offset + 2) = m12;
    }
    
    double Matrix2::M22() const
    {
        return source->at(offset + 3);
    }
    
    void Matrix2::SetM22(double m22)
    {
        source->at(offset + 3) = m22;
    }

    void Matrix2::Set(double m11, double m21, double m12, double m22)
    {
        SetM11(m11);
        SetM21(m21);
        SetM12(m12);
        SetM22(m22);
    }
    
    void Matrix2::Set(const Matrix2& source)
    {
        Set(source.M11(), source.M21(), source.M12(), source.M22());
    }
    
    void Matrix2::Premultiply(const Matrix2& left, Matrix2& result) const
    {
        result.Set(left.M11() * M11() + left.M21() * M12(), left.M11() * M21() + left.M21() * M22(), left.M12() * M11() + left.M22() * M12(), left.M12() * M21() + left.M22() * M22());
    }
    
    void Matrix2::Postmultiply(const Matrix2& right, Matrix2& result) const
    {
        result.Set(M11() * right.M11() + M21() * right.M12(), M11() * right.M21() + M21() * right.M22(), M12() * right.M11() + M22() * right.M12(), M12() * right.M21() + M22() * right.M22());
    }

    void Matrix2::Postmultiply(const Vector2& right, Vector2& result) const
    {
        result.Set(M11() * right.X() + M12() * right.Y(), M21() * right.X() + M22() * right.Y());
    }
    
    Matrix2& Matrix2::operator=(const Matrix2& right)
    {
        Set(right);
        
        return *this;
    }

    Matrix2::~Matrix2()
    {
        if (ownsSource)
        {
            delete source;
        }
    }
    
    Matrix2 operator*(const Matrix2& left, const Matrix2& right)
    {
        Matrix2 result;
        
        left.Postmultiply(right, result);
        
        return result;
    }
    
    Vector2 operator*(const Matrix2& left, const Vector2& right)
    {
        Vector2 result;
        
        left.Postmultiply(right, result);
        
        return result;
    }
}
