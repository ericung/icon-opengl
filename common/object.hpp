#ifndef SHAPE_HPP
#define SHAPE_HPP

class Object{
public:
    GLfloat buffer_data[36*3];
    GLuint buffer;
    int data;
    Object* next;
};

void iGLCubeGeometry(GLfloat* vertex_buffer, GLfloat width, GLfloat x, GLfloat y, GLfloat z);


void iGLCubeColor(GLfloat* color_buffer, GLfloat red, GLfloat green, GLfloat blue);

#endif
