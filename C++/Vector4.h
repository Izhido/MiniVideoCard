//
//  Vector4.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <vector>
#include "Vector3.h"

namespace MiniVideoCard
{
    using namespace std;
    
    class Vector4
    {
    public:
        Vector4();
        
        Vector4(double x, double y, double z);
        
        Vector4(double x, double y, double z, double w);
        
        Vector4(vector<double>* source, size_t offset);
        
        Vector4(const Vector4& source);
        
        Vector4(Vector4&& source);
        
        Vector4(Vector3& source);

        Vector4(Vector3& source, double w);
        
        Vector4(Vector2& source, double z);
        
        Vector4(Vector2& source, double z, double w);
        
		vector<double>* Source();

		size_t Offset();

		double X() const;
        
        void SetX(double x);
        
        double Y() const;
        
        void SetY(double y);
        
        double Z() const;
        
        void SetZ(double z);
        
        double W() const;
        
        void SetW(double w);
        
        void Set(double x, double y, double z);
        
        void Set(double x, double y, double z, double w);
        
        void Set(const Vector4& source);
        
        void Set(Vector3& source);
        
        void Set(Vector3& source, double w);
        
        void Set(Vector2& source, double z);
        
        void Set(Vector2& source, double z, double w);
        
		Vector2 XY();

		Vector3 XYZ();

        void Add(const Vector4& right, Vector4& result) const;
        
        void Subtract(const Vector4& right, Vector4& result) const;
        
        void Multiply(double right, Vector4& result) const;
        
        void Divide(double right, Vector4& result) const;
        
        double Dot(const Vector4& right) const;
        
        double SquareLength();
        
        double Length();
        
        bool Normalize();
        
        Vector4& operator=(const Vector4& right);
        
        ~Vector4();
        
    private:
        vector<double>* source;
        
        bool ownsSource;
        
        size_t offset;
    };
    
    Vector4 operator+(const Vector4& left, const Vector4& right);
    
    Vector4 operator-(const Vector4& left, const Vector4& right);
    
    Vector4 operator*(const Vector4& left, double right);
    
    double operator*(const Vector4& left, const Vector4& right);
    
    Vector4 operator/(const Vector4& left, double right);
}
