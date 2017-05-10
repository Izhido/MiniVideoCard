//
//  Matrix2.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <vector>
#include "Vector2.h"

namespace MiniVideoCard
{
    using namespace std;
    
    class Matrix2
    {
    public:
        Matrix2();
        
        Matrix2(double m11, double m21, double m12, double m22);
        
        Matrix2(vector<double>* source, size_t offset);
        
        Matrix2(const Matrix2& source);
        
        Matrix2(Matrix2&& source);
        
		vector<double>* Source();

		size_t Offset();

		double M11() const;
        
        void SetM11(double m11);
        
        double M21() const;
        
        void SetM21(double m21);
        
        double M12() const;
        
        void SetM12(double m12);
        
        double M22() const;
        
        void SetM22(double m22);

        void Set(double m11, double m21, double m12, double m22);
        
        void Set(const Matrix2& source);
        
        void Premultiply(const Matrix2& left, Matrix2& result) const;
        
        void Postmultiply(const Matrix2& right, Matrix2& result) const;
        
        void Postmultiply(const Vector2& right, Vector2& result) const;
        
        Matrix2& operator=(const Matrix2& right);
        
        ~Matrix2();
        
    private:
        vector<double>* source;
        
        bool ownsSource;
        
        size_t offset;
    };
    
    Matrix2 operator*(const Matrix2& left, const Matrix2& right);
    
    Vector2 operator*(const Matrix2& left, const Vector2& right);
}
