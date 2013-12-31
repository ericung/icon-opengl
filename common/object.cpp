// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
    
#include "object.hpp"


void iGLCubeGeometry(GLfloat* vertex_buffer, GLfloat width, GLfloat x, GLfloat y, GLfloat z){
    GLfloat copy_vertex_buffer[36*3] = {
        -width+x,-width+y,-width+z, // triangle 1 : begin
        -width+x,-width+y, width+z,
        -width+x, width+y, width+z, // triangle 1 : end

        width+x, width+y,-width+z, // triangle 2 : begin
        -width+x,-width+y,-width+z,
        -width+x, width+y,-width+z, // triangle 2 : end

        width+x,-width+y, width+z,
        -width+x,-width+y,-width+z,
        width+x,-width+y,-width+z,

        width+x, width+y,-width+z,
        width+x,-width+y,-width+z,
        -width+x,-width+y,-width+z,

        -width+x,-width+y,-width+z,
        -width+x, width+y, width+z,
        -width+x, width+y,-width+z,

        width+x,-width+y, width+z,
        -width+x,-width+y, width+z,
        -width+x,-width+y,-width+z,

        -width+x, width+y, width+z,
        -width+x,-width+y, width+z,
        width+x,-width+y, width+z,

        width+x, width+y, width+z,
        width+x,-width+y,-width+z,
        width+x, width+y,-width+z,

        width+x,-width+y,-width+z,
        width+x, width+y, width+z,
        width+x,-width+y, width+z,

        width+x, width+y, width+z,
        width+x, width+y,-width+z,
        -width+x, width+y,-width+z,

        width+x, width+y, width+z,
        -width+x, width+y,-width+z,
        -width+x, width+y, width+z,

        width+x, width+y, width+z,
        -width+x, width+y, width+z,
        width+x,-width+y, width+z
    };

    for (int i=0; i < 36*3; i++){
        vertex_buffer[i] = copy_vertex_buffer[i]; 
    }

}

void iGLCubeColor(GLfloat* color_buffer, GLfloat red, GLfloat green, GLfloat blue){
    GLfloat scale = 1.0f;
    for (int i=0; i<36*3; i+=3){
        color_buffer[i] = red*scale;
        color_buffer[i+1] = green*scale;
        color_buffer[i+2] = blue*scale;
        scale *= 0.98f;
    }
}
