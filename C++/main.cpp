//
//  main.cpp
//  TestMiniVideoCard
//
//  Created by Heriberto Delgado on 4/22/17.
//  Copyright © 2017 Heriberto Delgado. All rights reserved.
//

#include "Data.h"
#include "VertexShader.h"
#include "Vertex.h"
#include "FragmentShader.h"
#include "Fragment.h"
#include "Vector4.h"
#include <fstream>
#include "Rasterizer.h"
#include "ClearColorBuffer.h"
#include "ClearDepthBuffer.h"
#include "UseVertexShader.h"
#include "UseFragmentShader.h"
#include "Draw.h"
#include "Sampler2.h"
#include "Texture2.h"
#include "Filter.h"
#include "Parameters.h"
#include "SetParameters.h"
#include <algorithm>
#include <string>
#include "Matrix4.h"

using namespace MiniVideoCard;
using namespace std;

class PassthroughData : public Data
{
    public: double x, y, z;
    
    PassthroughData(double x, double y, double z) : x(x), y(y), z(z) {}
};

class PassthroughVertexShader : public VertexShader
{
    public: void Run(Data* data, Vertex* vertex, Parameters* parameters)
    {
        auto d = (PassthroughData *)data;
        
        vertex->Position().Set(d->x, d->y, d->z);
    }
};

class SingleColorFragmentShader : public FragmentShader
{
    public: double r, g, b, a;
    
    SingleColorFragmentShader(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {}

    void Run(Fragment* fragment, Parameters* parameters)
    {
        fragment->Color().Set(r, g, b, a);
    }
};

class GradientData : public Data
{
    public: double x, y, z, r, g, b, a;
    
    GradientData(double x, double y, double z, double r, double g, double b, double a) : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}
};

class GradientVertexShader : public VertexShader
{
    public: void Run(Data* data, Vertex* vertex, Parameters* parameters)
    {
        auto d = (GradientData *)data;
        
        vertex->Position().Set(d->x, d->y, d->z);
        
        vertex->Varyings().resize(4);

        Vector4 color(&vertex->Varyings(), 0);
        
        color.SetX(d->r);
        color.SetY(d->g);
        color.SetZ(d->b);
        color.SetW(d->a);
    }
};

class GradientFragmentShader : public FragmentShader
{
    public: void Run(Fragment* fragment, Parameters* parameters)
    {
        Vector4 color(&fragment->Varyings(), 0);
        
        fragment->Color().Set(color);
    }
};

class TexturedData : public Data
{
    public: double x, y, z, s, t;
    
    TexturedData(double x, double y, double z, double s, double t) : x(x), y(y), z(z), s(s), t(t) {}
};

class TexturedParameters : public Parameters
{
    public: Matrix4* model; Matrix4* view; Matrix4* projection; Texture2* texture; Sampler2* sampler;
    
    TexturedParameters(Matrix4* model, Matrix4* view, Matrix4* projection, Texture2* texture, Sampler2* sampler) : model(model), view(view), projection(projection), texture(texture), sampler(sampler) {}
};

class TexturedVertexShader : public VertexShader
{
    public: void Run(Data* data, Vertex* vertex, Parameters* parameters)
    {
        auto d = (TexturedData *)data;
        
        auto p = (TexturedParameters *)parameters;
        
        vertex->Position() = *p->projection * (*p->view * (*p->model * Vector4(d->x, d->y, d->z)));
        
        vertex->Varyings().resize(2);
        
        Vector2 texcoords(&vertex->Varyings(), 0);
        
        texcoords.SetX(d->s);
        texcoords.SetY(d->t);
    }
};

class TexturedFragmentShader : public FragmentShader
{
    public: void Run(Fragment* fragment, Parameters* parameters)
    {
        Vector2 texcoords(&fragment->Varyings(), 0);
        
        auto p = (TexturedParameters *)parameters;
        
        p->sampler->Sample(*p->texture, texcoords, *fragment, fragment->Color());
    }
};

void WriteInt16(size_t value, fstream& stream)
{
    stream.put((char)((unsigned char)(value & 255)));
    stream.put((char)((unsigned char)((value >> 8) & 255)));
}

void WriteInt32(size_t value, fstream& stream)
{
    stream.put((char)((unsigned char)(value & 255)));
    stream.put((char)((unsigned char)((value >> 8) & 255)));
    stream.put((char)((unsigned char)((value >> 16) & 255)));
    stream.put((char)((unsigned char)((value >> 24) & 255)));
}

void WriteNBytes(size_t value, size_t amount, fstream& stream)
{
    for (size_t i = 0; i < amount; i++)
    {
        stream.put((char)((unsigned char)(value & 255)));
    }
}

void WriteBitmap(size_t width, size_t height, vector<double>& buffer, size_t offset, string output)
{
    auto widthTimes4 = width * 4;
    
    auto size = widthTimes4 * height;
    
    fstream stream(output.c_str(), ios_base::out | ios_base::binary);

    stream.put('B');
    stream.put('M');
    WriteInt32(138 + size, stream);
    WriteNBytes(0, 2 + 2, stream);
    WriteInt32(138, stream);
    
    WriteInt32(124, stream);
    WriteInt32(width, stream);
    WriteInt32(height, stream);
    WriteInt16(1, stream);
    WriteInt16(32, stream);
    WriteInt32(3, stream);
    WriteInt32(size, stream);
    WriteInt32(2835, stream);
    WriteInt32(2835, stream);
    WriteNBytes(0, 4 + 4, stream);
    WriteInt32(255 << 16, stream);
    WriteInt32(255 << 8, stream);
    WriteInt32(255, stream);
    WriteInt32(255 << 24, stream);
    stream.put('B');
    stream.put('G');
    stream.put('R');
    stream.put('s');
    WriteNBytes(0, 36 + 4 * 3 + 4 * 4, stream);
    
    auto position = offset + (height - 1) * widthTimes4;

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            auto value = buffer[position + 2];
            
            value = max(min(value, 1.0), 0.0);
            
            stream.put((char)((unsigned char)(value * 255.0)));
            
            value = buffer[position + 1];
            
            value = max(min(value, 1.0), 0.0);
            
			stream.put((char)((unsigned char)(value * 255.0)));

            value = buffer[position];
            
            value = max(min(value, 1.0), 0.0);
            
			stream.put((char)((unsigned char)(value * 255.0)));

            value = buffer[position + 3];
            
            value = max(min(value, 1.0), 0.0);
            
			stream.put((char)((unsigned char)(value * 255.0)));

            position += 4;
        }
        
        position -= widthTimes4;
        position -= widthTimes4;
    }
}

int main(int argc, const char * argv[])
{
    Rasterizer rasterizer(1024, 768);
    
    vector<Data*> vertices;
    
    vertices.push_back(new PassthroughData(-0.5, -0.5, 0.5));
    vertices.push_back(new PassthroughData(-0.5, 0.5, 0.5));
    vertices.push_back(new PassthroughData(0.25, 0, -0.5));
    vertices.push_back(new PassthroughData(0.5, 0.5, 0.5));
    vertices.push_back(new PassthroughData(0.5, -0.5, 0.5));
    vertices.push_back(new PassthroughData(-0.25, 0, -0.5));
    
    rasterizer.AddCommand(true, new ClearColorBuffer(0, 0, 1, 0));
    rasterizer.AddCommand(true, new ClearDepthBuffer());
    rasterizer.AddCommand(true, new UseVertexShader(true, new PassthroughVertexShader()));
    rasterizer.AddCommand(true, new UseFragmentShader(true, new SingleColorFragmentShader(0, 1, 0, 0.5)));
    rasterizer.AddCommand(true, new Draw(false, &vertices, 0, 3));
    rasterizer.AddCommand(true, new UseFragmentShader(true, new SingleColorFragmentShader(1, 0, 0, 0.5)));
    rasterizer.AddCommand(true, new Draw(false, &vertices, 3, 3));
    
    rasterizer.Run();
    
    WriteBitmap(rasterizer.Width(), rasterizer.Height(), rasterizer.ColorBuffer(), 0, "test.bmp");
    
    vertices.push_back(new GradientData(-0.5, -0.5, 0.5, 1, 0, 0, 1));
    vertices.push_back(new GradientData(-0.5, 0.5, 0.5, 0, 1, 0, 1));
    vertices.push_back(new GradientData(0.25, 0, -0.5, 0, 0, 1, 0.2));
    vertices.push_back(new GradientData(0.5, 0.5, 0.5, 0, 0, 1, 1));
    vertices.push_back(new GradientData(0.5, -0.5, 0.5, 0, 1, 0, 1));
    vertices.push_back(new GradientData(-0.25, 0, -0.5, 1, 0, 0, 0.2));

    rasterizer.AddCommand(true, new ClearColorBuffer(0, 0, 0, 1));
    rasterizer.AddCommand(true, new ClearDepthBuffer());
    rasterizer.AddCommand(true, new UseVertexShader(true, new GradientVertexShader()));
    rasterizer.AddCommand(true, new UseFragmentShader(true, new GradientFragmentShader()));
    rasterizer.AddCommand(true, new Draw(false, &vertices, 6, 6));

    rasterizer.Run();
    
    WriteBitmap(rasterizer.Width(), rasterizer.Height(), rasterizer.ColorBuffer(), 0, "test2.bmp");
    
	Sampler2 sampler1;
	sampler1.SetMinificationFilter(NearestNeighbor);

    Texture2 texture1(rasterizer, true, &sampler1);
    
    for (size_t i = 0; i < texture1.Levels(); i++)
    {
        WriteBitmap(texture1.Width(i), texture1.Height(i), texture1.Buffer(), texture1.Offset(i), "test3_L" + to_string(i) + ".bmp");
    }

    Sampler2 sampler2;
	sampler2.SetMinificationFilter(Bilinear);

	Texture2 texture2(rasterizer, true, &sampler2);
    
    for (size_t i = 0; i < texture2.Levels(); i++)
    {
        WriteBitmap(texture2.Width(i), texture2.Height(i), texture2.Buffer(), texture2.Offset(i), "test4_L" + to_string(i) + ".bmp");
    }

    TexturedVertexShader texturedVertexShader;
    TexturedFragmentShader texturedFragmentShader;
    
    Matrix4 model;
    model.SetIdentity();
    
    Matrix4 view;
    view.SetLookAt(Vector3(0, 3, 3), Vector3(0, 0, 0), Vector3(0, 1, -1));
    
    Matrix4 projection;
    projection.SetPerspective(3.14159265358979323846 / 4.0, (double)rasterizer.Width() / (double)rasterizer.Height(), 0.1, 10);
    
	TexturedParameters texturedParameters1(&model, &view, &projection, &texture1, &sampler1);
    TexturedParameters texturedParameters2(&model, &view, &projection, &texture2, &sampler2);
    
    vertices.push_back(new TexturedData(0, 0.5, 0.5, -1.5, -1.5));
    vertices.push_back(new TexturedData(-0.5, 0.5, 0, 1.5, -1.5));
    vertices.push_back(new TexturedData(-0.5, -0.5, 0, 1.5, 1.5));
    vertices.push_back(new TexturedData(0, -0.5, 0.5, -1.5, 1.5));
    vertices.push_back(new TexturedData(-0.5, 0.5, 0, -1.5, -1.5));
    vertices.push_back(new TexturedData(0, 0.5, -0.5, 1.5, -1.5));
    vertices.push_back(new TexturedData(0, -0.5, -0.5, 1.5, 1.5));
    vertices.push_back(new TexturedData(-0.5, -0.5, 0, -1.5, 1.5));
    vertices.push_back(new TexturedData(0, 0.5, -0.5, -1.5, -1.5));
    vertices.push_back(new TexturedData(0.5, 0.5, 0, 1.5, -1.5));
    vertices.push_back(new TexturedData(0.5, -0.5, 0, 1.5, 1.5));
    vertices.push_back(new TexturedData(0, -0.5, -0.5, -1.5, 1.5));
    vertices.push_back(new TexturedData(0.5, 0.5, 0, -1.5, -1.5));
    vertices.push_back(new TexturedData(0, 0.5, 0.5, 1.5, -1.5));
    vertices.push_back(new TexturedData(0, -0.5, 0.5, 1.5, 1.5));
    vertices.push_back(new TexturedData(0.5, -0.5, 0, -1.5, 1.5));

    vector<size_t> indices;
    
    indices.push_back(12);
    indices.push_back(13);
    indices.push_back(14);
    indices.push_back(14);
    indices.push_back(15);
    indices.push_back(12);
    indices.push_back(16);
    indices.push_back(17);
    indices.push_back(18);
    indices.push_back(18);
    indices.push_back(19);
    indices.push_back(16);
    indices.push_back(20);
    indices.push_back(21);
    indices.push_back(22);
    indices.push_back(22);
    indices.push_back(23);
    indices.push_back(20);
    indices.push_back(24);
    indices.push_back(25);
    indices.push_back(26);
    indices.push_back(26);
    indices.push_back(27);
    indices.push_back(24);
    
    rasterizer.AddCommand(true, new ClearColorBuffer(0.5, 0.5, 0.5, 1));
    rasterizer.AddCommand(true, new ClearDepthBuffer());
    rasterizer.AddCommand(true, new UseVertexShader(false, &texturedVertexShader));
    rasterizer.AddCommand(true, new UseFragmentShader(false, &texturedFragmentShader));
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters1));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 0, 12));
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters2));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 12, 12));
    
    rasterizer.Run();
    
    WriteBitmap(rasterizer.Width(), rasterizer.Height(), rasterizer.ColorBuffer(), 0, "test5.bmp");

    sampler1.SetHorizontalWrap(MirroredRepeat);
    sampler1.SetVerticalWrap(MirroredRepeat);

    sampler2.SetHorizontalWrap(MirroredRepeat);
    sampler2.SetVerticalWrap(MirroredRepeat);
    
    rasterizer.AddCommand(true, new ClearColorBuffer(0.5, 0.5, 0.5, 1));
    rasterizer.AddCommand(true, new ClearDepthBuffer());
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters1));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 0, 12));
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters2));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 12, 12));
    
    rasterizer.Run();
    
    WriteBitmap(rasterizer.Width(), rasterizer.Height(), rasterizer.ColorBuffer(), 0, "test6.bmp");

    sampler1.SetHorizontalWrap(ClampToEdge);
    sampler1.SetVerticalWrap(ClampToEdge);

    sampler2.SetHorizontalWrap(ClampToEdge);
    sampler2.SetVerticalWrap(ClampToEdge);
    
    rasterizer.AddCommand(true, new ClearColorBuffer(0.5, 0.5, 0.5, 1));
    rasterizer.AddCommand(true, new ClearDepthBuffer());
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters1));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 0, 12));
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters2));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 12, 12));
    
    rasterizer.Run();
    
    WriteBitmap(rasterizer.Width(), rasterizer.Height(), rasterizer.ColorBuffer(), 0, "test7.bmp");

    sampler1.SetHorizontalWrap(ClampToBorder);
    sampler1.SetVerticalWrap(ClampToBorder);
    
    sampler1.Border().Set(1.0, 1.0, 0.0);

    sampler2.SetHorizontalWrap(ClampToBorder);
    sampler2.SetVerticalWrap(ClampToBorder);
    
    sampler2.Border().Set(0.0, 1.0, 1.0);
    
    rasterizer.AddCommand(true, new ClearColorBuffer(0.5, 0.5, 0.5, 1));
    rasterizer.AddCommand(true, new ClearDepthBuffer());
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters1));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 0, 12));
    rasterizer.AddCommand(true, new SetParameters(false, &texturedParameters2));
    rasterizer.AddCommand(true, new Draw(false, &vertices, false, &indices, 12, 12));
    
    rasterizer.Run();
    
    WriteBitmap(rasterizer.Width(), rasterizer.Height(), rasterizer.ColorBuffer(), 0, "test8.bmp");
    
    return 0;
}
