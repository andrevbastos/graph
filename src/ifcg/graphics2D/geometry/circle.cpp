#include <math.h>
#include "ifcg/graphics2D/geometry/circle.hpp"

namespace mesh2D
{
    Circle2D::Circle2D(int precision, GLuint shaderID)
        : Mesh2D(
            Circle2D::vertices(precision),
            Circle2D::indices(precision),
            shaderID
        ) {}

    std::vector<Vertex> Circle2D::vertices(int precision) {
        std::vector<Vertex> vertices;
        vertices.reserve(precision + 1);
        
        vertices.push_back(Vertex{ 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f });
    
        float angleStep = 2.0f * 3.1415926f / precision;
        
        for (int i = 0; i < precision; i++) {
            float currentAngle = i * angleStep;
            float x = cos(currentAngle);
            float y = sin(currentAngle);
            
            float hue = currentAngle / (2.0f * 3.1415926f);
            float r, g, b;
            
            float h = hue * 6.0f;
            int sector = static_cast<int>(h);
            float fraction = h - sector;
            float p = 1.0f;
            float q = 1.0f - fraction;
            float t = fraction;
            
            switch(sector) {
                case 0: r = 1; g = t; b = 0; break;
                case 1: r = q; g = 1; b = 0; break;
                case 2: r = 0; g = 1; b = t; break;
                case 3: r = 0; g = q; b = 1; break;
                case 4: r = t; g = 0; b = 1; break;
                default: r = 1; g = 0; b = q;
            }
            
            vertices.push_back(Vertex{ x, y, r, g, b, 1.0f });
        }
    
        return vertices;
    }

    std::vector<GLuint> Circle2D::indices(int precision) {
        std::vector<GLuint> indices;
        indices.reserve(precision * 3);
        
        for (int i = 1; i <= precision; i++) {
            indices.push_back(0);
            indices.push_back(i);
            
            if (i < precision) {
                indices.push_back(i + 1);
            } else {
                indices.push_back(1);
            }
        }

        return indices;
    }
}