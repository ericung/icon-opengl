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

// Include shaders
#include "common/loadshader.hpp"
#include "common/loadbmp.hpp"

class Object{
public:
    GLfloat buffer_data[36*3];
    GLuint buffer;
    Object* next;
};

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

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);           // 4x antialiasing
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);   // OpenGL 3.3
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);   
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // no old opengl

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}


	// Initialize GLEW
    glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "Icon OpenGL" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Object* cubeGeoms[2];
    for (int i=0; i<2; i++){
        cubeGeoms[i] = new Object();
        iGLCubeGeometry((cubeGeoms[i])->buffer_data, 0.5, i*2, 0.0f, 0.0f);
        
        glGenBuffers(1, &((cubeGeoms[i])->buffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (cubeGeoms[i])->buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,   sizeof(cubeGeoms[i]->buffer_data),
                                                cubeGeoms[i]->buffer_data, GL_STATIC_DRAW); 
    }

    // Generate a material
    GLfloat g_color_buffer_data[36*3];
    iGLCubeColor(g_color_buffer_data, 0.2f, 0.8f, 0.1f);
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,   sizeof(g_color_buffer_data), 
                                    g_color_buffer_data, GL_STATIC_DRAW);

    // Create and compile glsl program from shaders
    GLuint programID = LoadShaders( "vs.glsl", "fs.glsl");


    // Projection matrix : 45 deg Field of View, 4:3 ratio, display range : 0.1 unit
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,-3),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 MVP = Projection * View * Model;

    // load matrix
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	do{
         // clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(programID);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

   
        for (int i=0; i<2; i++){
            // gl vertexbuffer
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, cubeGeoms[i]->buffer);

            glVertexAttribPointer(
                0,
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // gl color
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glVertexAttribPointer(
                1,                      
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*) 0
            );

            // draw object
            glDrawArrays(GL_TRIANGLES, 0, 12*3); 
        }
                                 
        // Attrib
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);


		// Swap buffers
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

