#ifndef MATERIAL_HPP
#define MATERIAL_HPP

class Material{
public:
    GLfloat buffer_data[36*3];
    GLuint buffer;
    int data;
    Material* next;
};

void addMaterial(Material* sceneHead);

#endif
