//
//  Vector2.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 4/24/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include <vector>

namespace MiniVideoCard
{
    using namespace std;
    
    class Vector2
    {
    public:
        Vector2();
        
        Vector2(double x, double y);
        
        Vector2(vector<double>* source, size_t offset);
        
        Vector2(const Vector2& source);
        
        Vector2(Vector2&& source);
        
		vector<double>* Source();

		size_t Offset();

        double X() const;
        
        void SetX(double x);
        
        double Y() const;
        
        void SetY(double y);
        
        void Set(double x, double y);
        
        void Set(const Vector2& source);
        
        void Add(const Vector2& right, Vector2& result) const;
        
        void Subtract(const Vector2& right, Vector2& result) const;
        
        void Multiply(double right, Vector2& result) const;
        
        void Divide(double right, Vector2& result) const;
        
        double Dot(const Vector2& right) const;
        
        double SquareLength();
        
        double Length();
        
        bool Normalize();
        
        Vector2& operator=(const Vector2& right);
        
        ~Vector2();
        
    private:
        vector<double>* source;
        
        bool ownsSource;
        
        size_t offset;
    };
    
    Vector2 operator+(const Vector2& left, const Vector2& right);
    
    Vector2 operator-(const Vector2& left, const Vector2& right);
    
    Vector2 operator*(const Vector2& left, double right);
    
    double operator*(const Vector2& left, const Vector2& right);
    
    Vector2 operator/(const Vector2& left, double right);
}
