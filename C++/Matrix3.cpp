//
//  Matrix3.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 3/23/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Matrix3.h"

namespace MiniVideoCard
{
    using namespace std;
    
    Matrix3::Matrix3()
    {
        source = new vector<double>(9);
        ownsSource = true;
        offset = 0;
    }

    Matrix3::Matrix3(double m11, double m21, double m31, double m12, double m22, double m32, double m13, double m23, double m33) : Matrix3()
    {
        Set(m11, m21, m31, m12, m22, m32, m13, m23, m33);
    }
    
    Matrix3::Matrix3(vector<double>* source, size_t offset)
    {
        if (source == nullptr)
        {
            throw runtime_error("Source cannot be null.");
        }
        
        if (offset + 9 > source->size())
        {
            throw runtime_error("Offset exceeds available source size.");
        }
        
        this->source = source;
        ownsSource = false;
        this->offset = offset;
    }
    
    Matrix3::Matrix3(const Matrix3& source) : Matrix3()
    {
        Set(source);
    }
    
    Matrix3::Matrix3(Matrix3&& source)
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
    
    Matrix3::Matrix3(Matrix2& source) : Matrix3()
    {
        Set(source);
    }

	vector<double>* Matrix3::Source()
	{
		return source;
	}

	size_t Matrix3::Offset()
	{
		return offset;
	}

	double Matrix3::M11() const
    {
        return source->at(offset);
    }
    
    void Matrix3::SetM11(double m11)
    {
        source->at(offset) = m11;
    }
    
    double Matrix3::M21() const
    {
        return source->at(offset + 1);
    }
    
    void Matrix3::SetM21(double m21)
    {
        source->at(offset + 1) = m21;
    }
    
    double Matrix3::M31() const
    {
        return source->at(offset + 2);
    }
    
    void Matrix3::SetM31(double m31)
    {
        source->at(offset + 2) = m31;
    }
    
    double Matrix3::M12() const
    {
        return source->at(offset + 3);
    }
    
    void Matrix3::SetM12(double m12)
    {
        source->at(offset + 3) = m12;
    }
    
    double Matrix3::M22() const
    {
        return source->at(offset + 3);
    }
    
    void Matrix3::SetM22(double m22)
    {
        source->at(offset + 3) = m22;
    }
    
    double Matrix3::M32() const
    {
        return source->at(offset + 5);
    }
    
    void Matrix3::SetM32(double m32)
    {
        source->at(offset + 5) = m32;
    }
    
    double Matrix3::M13() const
    {
        return source->at(offset + 6);
    }
    
    void Matrix3::SetM13(double m13)
    {
        source->at(offset + 6) = m13;
    }
    
    double Matrix3::M23() const
    {
        return source->at(offset + 7);
    }
    
    void Matrix3::SetM23(double m23)
    {
        source->at(offset + 7) = m23;
    }
    
    double Matrix3::M33() const
    {
        return source->at(offset + 8);
    }
    
    void Matrix3::SetM33(double m33)
    {
        source->at(offset + 8) = m33;
    }

    void Matrix3::Set(double m11, double m21, double m31, double m12, double m22, double m32, double m13, double m23, double m33)
    {
        SetM11(m11);
        SetM21(m21);
        SetM31(m31);
        SetM12(m12);
        SetM22(m22);
        SetM32(m32);
        SetM13(m13);
        SetM23(m23);
        SetM33(m33);
    }
    
    void Matrix3::Set(const Matrix3& source)
    {
        Set(source.M11(), source.M21(), source.M31(), source.M12(), source.M22(), source.M32(), source.M13(), source.M23(), source.M33());
    }

    void Matrix3::Set(Matrix2& source)
    {
        Set(source.M11(), source.M21(), 0, source.M12(), source.M22(), 0, 0, 0, 1);
    }
    
    void Matrix3::Premultiply(const Matrix3& left, Matrix3& result) const
    {
        result.Set(left.M11() * M11() + left.M21() * M12() + left.M31() * M13(), left.M11() * M21() + left.M21() * M22() + left.M31() * M23(), left.M11() * M31() + left.M21() * M32() + left.M31() * M33(), left.M12() * M11() + left.M22() * M12() + left.M32() * M13(), left.M12() * M21() + left.M22() * M22() + left.M32() * M23(), left.M12() * M31() + left.M22() * M32() + left.M32() * M33(), left.M13() * M11() + left.M23() * M12() + left.M33() * M13(), left.M13() * M21() + left.M23() * M22() + left.M33() * M23(), left.M13() * M31() + left.M23() * M32() + left.M33() * M33());
    }
    
    void Matrix3::Postmultiply(const Matrix3& right, Matrix3& result) const
    {
        result.Set(M11() * right.M11() + M21() * right.M12() + M31() * right.M13(), M11() * right.M21() + M21() * right.M22() + M31() * right.M23(), M11() * right.M31() + M21() * right.M32() + M31() * right.M33(), M12() * right.M11() + M22() * right.M12() + M32() * right.M13(), M12() * right.M21() + M22() * right.M22() + M32() * right.M23(), M12() * right.M31() + M22() * right.M32() + M32() * right.M33(), M13() * right.M11() + M23() * right.M12() + M33() * right.M13(), M13() * right.M21() + M23() * right.M22() + M33() * right.M23(), M13() * right.M31() + M23() * right.M32() + M33() * right.M33());
    }
    
    void Matrix3::Postmultiply(const Vector3& right, Vector3& result) const
    {
        result.Set(M11() * right.X() + M12() * right.Y() + M13() * right.Z(), M21() * right.X() + M22() * right.Y() + M23() * right.Z(), M31() * right.X() + M32() * right.Y() + M33() * right.Z());
    }
    
    Matrix3& Matrix3::operator=(const Matrix3& right)
    {
        Set(right);
        
        return *this;
    }

    Matrix3::~Matrix3()
    {
        if (ownsSource)
        {
            delete source;
        }
    }
    
    Matrix3 operator*(const Matrix3& left, const Matrix3& right)
    {
        Matrix3 result;
        
        left.Postmultiply(right, result);
        
        return result;
    }
    
    Vector3 operator*(const Matrix3& left, const Vector3& right)
    {
        Vector3 result;
        
        left.Postmultiply(right, result);
        
        return result;
    }
}
