//
//  Matrix4.cpp
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Matrix4.h"
#include <cmath>

namespace MiniVideoCard
{
    using namespace std;
    
    Matrix4::Matrix4()
    {
        source = new vector<double>(16);
        ownsSource = true;
        offset = 0;
    }

    Matrix4::Matrix4(double m11, double m21, double m31, double m41, double m12, double m22, double m32, double m42, double m13, double m23, double m33, double m43, double m14, double m24, double m34, double m44) : Matrix4()
    {
        Set(m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44);
    }
    
    Matrix4::Matrix4(vector<double>* source, size_t offset)
    {
        if (source == nullptr)
        {
            throw runtime_error("Source cannot be null.");
        }
        
        if (offset + 16 > source->size())
        {
            throw runtime_error("Offset exceeds available source size.");
        }
        
        this->source = source;
        ownsSource = false;
        this->offset = offset;
    }
    
    Matrix4::Matrix4(const Matrix4& source) : Matrix4()
    {
        Set(source);
    }
    
    Matrix4::Matrix4(Matrix4&& source)
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
    
    Matrix4::Matrix4(Matrix3& source) : Matrix4()
    {
        Set(source);
    }
    
    Matrix4::Matrix4(Matrix2& source) : Matrix4()
    {
        Set(source);
    }
    
	vector<double>* Matrix4::Source()
	{
		return source;
	}

	size_t Matrix4::Offset()
	{
		return offset;
	}

	double Matrix4::M11() const
    {
        return source->at(offset);
    }
    
    void Matrix4::SetM11(double m11)
    {
        source->at(offset) = m11;
    }
    
    double Matrix4::M21() const
    {
        return source->at(offset + 1);
    }
    
    void Matrix4::SetM21(double m21)
    {
        source->at(offset + 1) = m21;
    }
    
    double Matrix4::M31() const
    {
        return source->at(offset + 2);
    }
    
    void Matrix4::SetM31(double m31)
    {
        source->at(offset + 2) = m31;
    }
    
    double Matrix4::M41() const
    {
        return source->at(offset + 3);
    }
    
    void Matrix4::SetM41(double m41)
    {
        source->at(offset + 3) = m41;
    }
    
    double Matrix4::M12() const
    {
        return source->at(offset + 4);
    }
    
    void Matrix4::SetM12(double m12)
    {
        source->at(offset + 4) = m12;
    }
    
    double Matrix4::M22() const
    {
        return source->at(offset + 5);
    }
    
    void Matrix4::SetM22(double m22)
    {
        source->at(offset + 5) = m22;
    }
    
    double Matrix4::M32() const
    {
        return source->at(offset + 6);
    }
    
    void Matrix4::SetM32(double m32)
    {
        source->at(offset + 6) = m32;
    }
    
    double Matrix4::M42() const
    {
        return source->at(offset + 7);
    }
    
    void Matrix4::SetM42(double m42)
    {
        source->at(offset + 7) = m42;
    }
    
    double Matrix4::M13() const
    {
        return source->at(offset + 8);
    }
    
    void Matrix4::SetM13(double m13)
    {
        source->at(offset + 8) = m13;
    }
    
    double Matrix4::M23() const
    {
        return source->at(offset + 9);
    }
    
    void Matrix4::SetM23(double m23)
    {
        source->at(offset + 9) = m23;
    }
    
    double Matrix4::M33() const
    {
        return source->at(offset + 10);
    }
    
    void Matrix4::SetM33(double m33)
    {
        source->at(offset + 10) = m33;
    }
    
    double Matrix4::M43() const
    {
        return source->at(offset + 11);
    }
    
    void Matrix4::SetM43(double m43)
    {
        source->at(offset + 11) = m43;
    }
    
    double Matrix4::M14() const
    {
        return source->at(offset + 12);
    }
    
    void Matrix4::SetM14(double m14)
    {
        source->at(offset + 12) = m14;
    }
    
    double Matrix4::M24() const
    {
        return source->at(offset + 13);
    }
    
    void Matrix4::SetM24(double m24)
    {
        source->at(offset + 13) = m24;
    }
    
    double Matrix4::M34() const
    {
        return source->at(offset + 14);
    }
    
    void Matrix4::SetM34(double m34)
    {
        source->at(offset + 14) = m34;
    }
    
    double Matrix4::M44() const
    {
        return source->at(offset + 15);
    }
    
    void Matrix4::SetM44(double m44)
    {
        source->at(offset + 15) = m44;
    }
    
    void Matrix4::Set(double m11, double m21, double m31, double m41, double m12, double m22, double m32, double m42, double m13, double m23, double m33, double m43, double m14, double m24, double m34, double m44)
    {
        SetM11(m11);
        SetM21(m21);
        SetM31(m31);
        SetM41(m41);
        SetM12(m12);
        SetM22(m22);
        SetM32(m32);
        SetM42(m42);
        SetM13(m13);
        SetM23(m23);
        SetM33(m33);
        SetM43(m43);
        SetM14(m14);
        SetM24(m24);
        SetM34(m34);
        SetM44(m44);
    }

    void Matrix4::Set(const Matrix4& source)
    {
        Set(source.M11(), source.M21(), source.M31(), source.M41(), source.M12(), source.M22(), source.M32(), source.M42(), source.M13(), source.M23(), source.M33(), source.M43(), source.M14(), source.M24(), source.M34(), source.M44());
    }
    
    void Matrix4::Set(Matrix3& source)
    {
        Set(source.M11(), source.M21(), source.M31(), 0, source.M12(), source.M22(), source.M32(), 0, source.M13(), source.M23(), source.M33(), 0, 0, 0, 0, 1);
    }
    
    void Matrix4::Set(Matrix2& source)
    {
        Set(source.M11(), source.M21(), 0, 0, source.M12(), source.M22(), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }
    
    void Matrix4::SetIdentity()
    {
        Set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }

    void Matrix4::SetTranslation(double x, double y, double z)
    {
        Set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1);
    }
    
    void Matrix4::SetTranslation(Vector2& translation)
    {
        SetTranslation(translation.X(), translation.Y(), 0);
    }
    
    void Matrix4::SetTranslation(Vector3& translation)
    {
        SetTranslation(translation.X(), translation.Y(), translation.Z());
    }
    
    void Matrix4::SetTranslation(Vector4& translation)
    {
        SetTranslation(translation.X(), translation.Y(), translation.Z());
    }
    
    void Matrix4::SetScale(double scale)
    {
        SetScale(scale, scale, scale);
    }
    
    void Matrix4::SetScale(double x, double y, double z)
    {
        Set(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
    }
    
    void Matrix4::SetScale(Vector2& scale)
    {
        SetScale(scale.X(), scale.Y(), 1);
    }
    
    void Matrix4::SetScale(Vector3& scale)
    {
        SetScale(scale.X(), scale.Y(), scale.Z());
    }
    
    void Matrix4::SetScale(Vector4& scale)
    {
        SetScale(scale.X(), scale.Y(), scale.Z());
    }
    
    void Matrix4::SetRotationX(double angle)
    {
        auto angleSin = sin(angle);
        auto angleCos = cos(angle);
        
        Set(1, 0, 0, 0, 0, angleCos, angleSin, 0, 0, -angleSin, angleCos, 0, 0, 0, 0, 1);
    }
    
    void Matrix4::SetRotationY(double angle)
    {
        auto angleSin = sin(angle);
        auto angleCos = cos(angle);
        
        Set(angleCos, 0, -angleSin, 0, 0, 1, 0, 0, angleSin, 0, angleCos, 0, 0, 0, 0, 1);
    }
    
    void Matrix4::SetRotationZ(double angle)
    {
        auto angleSin = sin(angle);
        auto angleCos = cos(angle);
        
        Set(angleCos, angleSin, 0, 0, -angleSin, angleCos, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }
    
    void Matrix4::SetRotation(double angle, const Vector3& direction)
    {
        auto angleSin = sin(angle);
        auto angleCos = cos(angle);
        auto oneMinusAngleCos = 1 - angleCos;
        
        auto normalized = direction;
        normalized.Normalize();
        
        Set(normalized.X() * normalized.X() * oneMinusAngleCos + angleCos, normalized.Y() * normalized.X() * oneMinusAngleCos + normalized.Z() * angleSin, normalized.Z() * normalized.X() * oneMinusAngleCos - normalized.Y() * angleSin, 0, normalized.X() * normalized.Y() * oneMinusAngleCos - normalized.Z() * angleSin, normalized.Y() * normalized.Y() * oneMinusAngleCos + angleCos, normalized.Z() * normalized.Y() * oneMinusAngleCos + normalized.X() * angleSin, 0, normalized.X() * normalized.Z() * oneMinusAngleCos + normalized.Y() * angleSin, normalized.Y() * normalized.Z() * oneMinusAngleCos - normalized.X() * angleSin, normalized.Z() * normalized.Z() * oneMinusAngleCos + angleCos, 0, 0, 0, 0, 1);
    }
    
    void Matrix4::SetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
    {
        auto f = center - eye;
        
        f.Normalize();
        
        Vector3 upNormalized = up;
        
        upNormalized.Normalize();
        
        auto s = f ^ upNormalized;
        
        Vector3 sNormalized = s;
        
        sNormalized.Normalize();
        
        auto u = sNormalized ^ f;
        
        auto minusEye = eye * -1;
        
        Set(s.X(), u.X(), -f.X(), 0, s.Y(), u.Y(), -f.Y(), 0, s.Z(), u.Z(), -f.Z(), 0, s * minusEye, u * minusEye, (f * -1) * minusEye, 1);
    }

    void Matrix4::SetPerspective(double verticalFieldOfView, double aspectRatio, double near, double far)
    {
        auto halfVerticalFieldOfView = verticalFieldOfView / 2;
        
        auto f = cos(halfVerticalFieldOfView) / sin(halfVerticalFieldOfView);
        
        auto nearMinusFar = near - far;
        
        Set(f / aspectRatio, 0, 0, 0, 0, f, 0, 0, 0, 0, (far + near) / nearMinusFar, -1, 0, 0, (2.0 * far * near) / nearMinusFar, 0);
    }
    
    void Matrix4::SetOrthogonal(double left, double top, double right, double bottom, double near, double far)
    {
        auto rightPlusLeft = right + left;
        auto rightMinusLeft = right - left;
        auto topPlusBottom = top + bottom;
        auto topMinusBottom = top - bottom;
        auto farPlusNear = far + near;
        auto farMinusNear = far - near;
        
        Set(2.0 / rightMinusLeft, 0, 0, 0, 0, 2.0 / topMinusBottom, 0, 0, 0, 0, -2.0 / farMinusNear, 0, -(rightPlusLeft / rightMinusLeft), -(topPlusBottom / topMinusBottom), -(farPlusNear / farMinusNear), 1);
    }
    
    void Matrix4::Premultiply(const Matrix4& left, Matrix4& result) const
    {
        result.Set(left.M11() * M11() + left.M21() * M12() + left.M31() * M13() + left.M41() * M14(), left.M11() * M21() + left.M21() * M22() + left.M31() * M23() + left.M41() * M24(), left.M11() * M31() + left.M21() * M32() + left.M31() * M33() + left.M41() * M34(), left.M11() * M41() + left.M21() * M42() + left.M31() * M43() + left.M41() * M44(), left.M12() * M11() + left.M22() * M12() + left.M32() * M13() + left.M42() * M14(), left.M12() * M21() + left.M22() * M22() + left.M32() * M23() + left.M42() * M24(), left.M12() * M31() + left.M22() * M32() + left.M32() * M33() + left.M42() * M34(), left.M12() * M41() + left.M22() * M42() + left.M32() * M43() + left.M42() * M44(), left.M13() * M11() + left.M23() * M12() + left.M33() * M13() + left.M43() * M14(), left.M13() * M21() + left.M23() * M22() + left.M33() * M23() + left.M43() * M24(), left.M13() * M31() + left.M23() * M32() + left.M33() * M33() + left.M43() * M34(), left.M13() * M41() + left.M23() * M42() + left.M33() * M43() + left.M43() * M44(), left.M14() * M11() + left.M24() * M12() + left.M34() * M13() + left.M44() * M14(), left.M14() * M21() + left.M24() * M22() + left.M34() * M23() + left.M44() * M24(), left.M14() * M31() + left.M24() * M32() + left.M34() * M33() + left.M44() * M34(), left.M14() * M41() + left.M24() * M42() + left.M34() * M43() + left.M44() * M44());
    }
    
    void Matrix4::Postmultiply(const Matrix4& right, Matrix4& result) const
    {
        result.Set(M11() * right.M11() + M21() * right.M12() + M31() * right.M13() + M41() * right.M14(), M11() * right.M21() + M21() * right.M22() + M31() * right.M23() + M41() * right.M24(), M11() * right.M31() + M21() * right.M32() + M31() * right.M33() + M41() * right.M34(), M11() * right.M41() + M21() * right.M42() + M31() * right.M43() + M41() * right.M44(), M12() * right.M11() + M22() * right.M12() + M32() * right.M13() + M42() * right.M14(), M12() * right.M21() + M22() * right.M22() + M32() * right.M23() + M42() * right.M24(), M12() * right.M31() + M22() * right.M32() + M32() * right.M33() + M42() * right.M34(), M12() * right.M41() + M22() * right.M42() + M32() * right.M43() + M42() * right.M44(), M13() * right.M11() + M23() * right.M12() + M33() * right.M13() + M43() * right.M14(), M13() * right.M21() + M23() * right.M22() + M33() * right.M23() + M43() * right.M24(), M13() * right.M31() + M23() * right.M32() + M33() * right.M33() + M43() * right.M34(), M13() * right.M41() + M23() * right.M42() + M33() * right.M43() + M43() * right.M44(), M14() * right.M11() + M24() * right.M12() + M34() * right.M13() + M44() * right.M14(), M14() * right.M21() + M24() * right.M22() + M34() * right.M23() + M44() * right.M24(), M14() * right.M31() + M24() * right.M32() + M34() * right.M33() + M44() * right.M34(), M14() * right.M41() + M24() * right.M42() + M34() * right.M43() + M44() * right.M44());
    }

    void Matrix4::Postmultiply(const Vector4& right, Vector4& result) const
    {
        result.Set(M11() * right.X() + M12() * right.Y() + M13() * right.Z() + M14() * right.W(), M21() * right.X() + M22() * right.Y() + M23() * right.Z() + M24() * right.W(), M31() * right.X() + M32() * right.Y() + M33() * right.Z() + M34() * right.W(), M41() * right.X() + M42() * right.Y() + M43() * right.Z() + M44() * right.W());
    }
    
    Matrix4& Matrix4::operator=(const Matrix4& right)
    {
        Set(right);
        
        return *this;
    }

    Matrix4::~Matrix4()
    {
        if (ownsSource)
        {
            delete source;
        }
    }
    
    Matrix4 operator*(const Matrix4& left, const Matrix4& right)
    {
        Matrix4 result;
        
        left.Postmultiply(right, result);
        
        return result;
    }
    
    Vector4 operator*(const Matrix4& left, const Vector4& right)
    {
        Vector4 result;
        
        left.Postmultiply(right, result);
        
        return result;
    }
}
