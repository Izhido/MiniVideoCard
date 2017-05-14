//
//  Matrix4.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <vector>
#include "Matrix3.h"
#include "Vector4.h"

namespace MiniVideoCard
{
    using namespace std;
    
    class Matrix4
    {
    public:
        Matrix4();
        
        Matrix4(double m11, double m21, double m31, double m41, double m12, double m22, double m32, double m42, double m13, double m23, double m33, double m43, double m14, double m24, double m34, double m44);
        
        Matrix4(vector<double>* source, size_t offset);
        
        Matrix4(const Matrix4& source);
        
        Matrix4(Matrix4&& source);
        
        Matrix4(Matrix3& source);
        
        Matrix4(Matrix2& source);
        
		vector<double>* Source();

		size_t Offset();

		double M11() const;
        
        void SetM11(double m11);
        
        double M21() const;
        
        void SetM21(double m21);
        
        double M31() const;
        
        void SetM31(double m31);
        
        double M41() const;
        
        void SetM41(double m41);
        
        double M12() const;
        
        void SetM12(double m12);
        
        double M22() const;
        
        void SetM22(double m22);
        
        double M32() const;
        
        void SetM32(double m32);
        
        double M42() const;
        
        void SetM42(double m42);
        
        double M13() const;
        
        void SetM13(double m13);
        
        double M23() const;
        
        void SetM23(double m23);
        
        double M33() const;
        
        void SetM33(double m33);
        
        double M43() const;
        
        void SetM43(double m43);
        
        double M14() const;
        
        void SetM14(double m14);
        
        double M24() const;
        
        void SetM24(double m24);
        
        double M34() const;
        
        void SetM34(double m34);
        
        double M44() const;
        
        void SetM44(double m44);
        
        void Set(double m11, double m21, double m31, double m41, double m12, double m22, double m32, double m42, double m13, double m23, double m33, double m43, double m14, double m24, double m34, double m44);
        
        void Set(const Matrix4& source);
        
        void Set(Matrix3& source);
        
        void Set(Matrix2& source);

        void SetIdentity();
        
        void SetTranslation(double x, double y, double z);
        
        void SetTranslation(Vector2& translation);
        
        void SetTranslation(Vector3& translation);
        
        void SetTranslation(Vector4& translation);
        
        void SetScale(double scale);
        
        void SetScale(double x, double y, double z);
        
        void SetScale(Vector2& scale);
        
        void SetScale(Vector3& scale);
        
        void SetScale(Vector4& scale);
        
        void SetRotationX(double angle);
        
        void SetRotationY(double angle);
        
        void SetRotationZ(double angle);
        
        void SetRotation(double angle, const Vector3& direction);
        
        void SetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
        
        void SetPerspective(double verticalFieldOfView, double aspectRatio, double near, double far);
        
        void SetOrthogonal(double left, double top, double right, double bottom, double near, double far);
        
        void Premultiply(const Matrix4& left, Matrix4& result) const;
        
        void Postmultiply(const Matrix4& right, Matrix4& result) const;
        
        void Postmultiply(const Vector4& right, Vector4& result) const;
        
        Matrix4& operator=(const Matrix4& right);
        
        ~Matrix4();
        
    private:
        vector<double>* source;
        
        bool ownsSource;
        
        size_t offset;
    };
    
    Matrix4 operator*(const Matrix4& left, const Matrix4& right);
    
    Vector4 operator*(const Matrix4& left, const Vector4& right);
}
