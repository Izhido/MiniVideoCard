//
//  Vector3.h
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
    
    class Vector3
    {
    public:
        Vector3();
        
        Vector3(double x, double y, double z);
        
        Vector3(vector<double>* source, size_t offset);
        
        Vector3(const Vector3& source);
        
        Vector3(Vector3&& source);
        
        Vector3(Vector2& source, double z);
        
		vector<double>* Source();

		size_t Offset();

		double X() const;
        
        void SetX(double x);
        
        double Y() const;
        
        void SetY(double y);
        
        double Z() const;
        
        void SetZ(double z);
        
        void Set(double x, double y, double z);
        
        void Set(const Vector3& source);
        
        void Set(Vector2& source, double z);
        
		Vector2 XY();

        void Add(const Vector3& right, Vector3& result) const;
        
        void Subtract(const Vector3& right, Vector3& result) const;
        
        void Multiply(double right, Vector3& result) const;
        
        void Divide(double right, Vector3& result) const;
        
        double Dot(const Vector3& right) const;
        
        void Cross(const Vector3& right, Vector3& result) const;
        
        double SquareLength();
        
        double Length();
        
        bool Normalize();

        Vector3& operator=(const Vector3& right);
        
        ~Vector3();
        
    private:
        vector<double>* source;
        
        bool ownsSource;
        
        size_t offset;
    };

    Vector3 operator+(const Vector3& left, const Vector3& right);
    
    Vector3 operator-(const Vector3& left, const Vector3& right);
    
    Vector3 operator*(const Vector3& left, double right);
    
    double operator*(const Vector3& left, const Vector3& right);
    
    Vector3 operator^(const Vector3& left, const Vector3& right);
    
    Vector3 operator/(const Vector3& left, double right);
}
