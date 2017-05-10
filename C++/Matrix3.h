//
//  Matrix3.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <vector>
#include "Matrix2.h"
#include "Vector3.h"

namespace MiniVideoCard
{
    using namespace std;
    
    class Matrix3
    {
    public:
        Matrix3();
        
        Matrix3(double m11, double m21, double m31, double m12, double m22, double m32, double m13, double m23, double m33);
        
        Matrix3(vector<double>* source, size_t offset);
        
        Matrix3(const Matrix3& source);
        
        Matrix3(Matrix3&& source);
        
        Matrix3(Matrix2& source);
        
		vector<double>* Source();

		size_t Offset();

		double M11() const;
        
        void SetM11(double m11);
        
        double M21() const;
        
        void SetM21(double m21);
        
        double M31() const;
        
        void SetM31(double m31);
        
        double M12() const;
        
        void SetM12(double m12);
        
        double M22() const;
        
        void SetM22(double m22);
        
        double M32() const;
        
        void SetM32(double m32);
        
        double M13() const;
        
        void SetM13(double m13);
        
        double M23() const;
        
        void SetM23(double m23);
        
        double M33() const;
        
        void SetM33(double m33);
        
        void Set(double m11, double m21, double m31, double m12, double m22, double m32, double m13, double m23, double m33);
        
        void Set(const Matrix3& source);
        
        void Set(Matrix2& source);
        
        void Premultiply(const Matrix3& left, Matrix3& result) const;
        
        void Postmultiply(const Matrix3& right, Matrix3& result) const;
        
        void Postmultiply(const Vector3& left, Vector3& result) const;
        
        Matrix3& operator=(const Matrix3& right);
        
        ~Matrix3();
        
    private:
        vector<double>* source;
        
        bool ownsSource;
        
        size_t offset;
    };
    
    Matrix3 operator*(const Matrix3& left, const Matrix3& right);
    
    Vector3 operator*(const Matrix3& left, const Vector3& right);
}
