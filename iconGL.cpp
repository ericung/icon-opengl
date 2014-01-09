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
#include "common/object.hpp"
#include "common/material.hpp"


extern "C" int send(value argv[])
{
    if( !argv[1].toString() ){
        Icon::runerr(103, argv[1]);
        return FAILED; 
    }

    safe newname(argv[1]);
    char* s = value(newname || nullchar);
    printf("%s", s);
    return SUCCEEDED;
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

	glfwSetWindowTitle( "Icon OpenGL" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // geometry list
    Object* geoList = new Object();
    addObject(geoList);
    addObject(geoList);
    addObject(geoList);
    addObject(geoList);
    addObject(geoList);

    // material list
    Material* matList = new Material();
    addMaterial(matList);

    Material* iterMat = matList;
    while(iterMat->next != NULL){
        iterMat = iterMat->next;

        // Generate a material
        iGLCubeColor(iterMat->buffer_data, 0.5f, 0.1f, 0.8f);
        
        glGenBuffers(1, &(iterMat->buffer));
        glBindBuffer(GL_ARRAY_BUFFER, iterMat->buffer);
        glBufferData(GL_ARRAY_BUFFER,   sizeof(iterMat->buffer_data), 
                                        iterMat->buffer_data, GL_STATIC_DRAW);
    }

    // 
    Object* iter = geoList;
    int i = 0;
    while(iter->next != NULL){
        iter = iter->next;
        iGLCubeGeometry(iter->buffer_data, 0.5, i, 0.0f, 0.0f);
        
        glGenBuffers(1, &(iter->buffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iter->buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,   sizeof(iter->buffer_data),
                                                iter->buffer_data, GL_STATIC_DRAW); 
        iter->material = matList->next;

        i++;
    }

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
   
        Object* iter = geoList;
        while(iter->next != NULL){
            // iter
            iter = iter->next;

            // gl vertexbuffer
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, iter->buffer);

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
            glBindBuffer(GL_ARRAY_BUFFER, (iter->material)->buffer);
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

