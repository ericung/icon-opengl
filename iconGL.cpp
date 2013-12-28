// Include standard headers
#include "loadfuncpp.h"
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

void CubeGeometry(GLfloat* vertex_buffer, GLfloat width, GLfloat x, GLfloat y, GLfloat z){
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

void CubeColor(GLfloat* color_buffer, GLfloat color){
/*
    GLfloat copy_color_buffer[36*3] = {
        color,  color,  color,
        color,  color,  color,
        color,  color,  color,
        color,  color,  color,
        color,  color,  color,
        color,  color,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };   
    */

    for (int i=0; i<36*3; i++){
        color_buffer[i] = color;
    }

}

//int main( void )
extern "C" int initGL(value argv[])
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

	glfwSetWindowTitle( "Tutorial 01" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Generate a cube geometry
    GLfloat width = 1.0f;
    GLfloat g_vertex_buffer_data[36*3];
    CubeGeometry(g_vertex_buffer_data, width,0, 0, 0);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    // Generate a material
    GLfloat g_color_buffer_data[36*3];
    CubeColor(g_color_buffer_data, 0.5f);
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER,   sizeof(g_color_buffer_data), 
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
   
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

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


        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
                         
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

