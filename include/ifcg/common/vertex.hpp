#pragma once

class Vertex {
public:
    Vertex(float x, float y, float z = 0.0f, float r = 1.0f, float g = 0.0f, float b = 1.0f, float a = 1.0f) 
        : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}

    float x, y, z;
    float r, g, b, a;
};