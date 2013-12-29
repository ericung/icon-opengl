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

void CubeGeometry(GLfloat vertex_buffer[36*3], GLfloat width){
    GLfloat copy_vertex_buffer[36*3] = {
        -width,-width,-width, // triangle 1 : begin
        -width,-width, width,
        -width, width, width, // triangle 1 : end
        width, width,-width, // triangle 2 : begin
        -width,-width,-width,
        -width, width,-width, // triangle 2 : end

        width,-width, width,
        -width,-width,-width,
        width,-width,-width,
        width, width,-width,
        width,-width,-width,
        -width,-width,-width,
        -width,-width,-width,
        -width, width, width,
        -width, width,-width,
        width,-width, width,

        -width,-width, width,
        -width,-width,-width,
        -width, width, width,
        -width,-width, width,
        width,-width, width,
        width, width, width,
        width,-width,-width,
        width, width,-width,
        width,-width,-width,
        width, width, width,

        width,-width, width,
        width, width, width,
        width, width,-width,
        -width, width,-width,
        width, width, width,
        -width, width,-width,
        -width, width, width,
        width, width, width,
        -width, width, width,
        width,-width, width
    };

    for (int i=0; i < 36*3; i++){
        vertex_buffer[i] = copy_vertex_buffer[i]; 
    }

}

int main( void )
//extern "C" int openWindow(value argv[])
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

	glfwSetWindowTitle( "Open Window" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile glsl program from shaders
    GLuint programID = LoadShaders( "vsbmp.glsl", "fsbmp.glsl");

    // Projection matrix : 45 deg Field of View, 4:3 ratio, display range : 0.1 unit
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // load matrix
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,-3),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 MVP = Projection * View * Model;

    //GLuint Texture = loadDDS("uvtemplate.DDS");
    //GLuint Texture = loadBMP_custom("uvtemplate.bmp");
    GLuint Texture = loadTGA_glfw("uvtemplate.tga");
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // Our vertices. Tree consecutive floats give a 3D vertex; 
    // Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    /*
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    */

    GLfloat g_vertex_buffer_data[36*3];
    CubeGeometry(g_vertex_buffer_data, 1.0f);

    // Two UV coordinatesfor each vertex. They were created with Blender. 
    // You'll learn shortly how to do this yourself.
    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), 
                                    g_vertex_buffer_data, GL_STATIC_DRAW);


    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER,   sizeof(g_uv_buffer_data), 
                                    g_uv_buffer_data, GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	do{
         // clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);
   
        // 1rst attribute buffer : vertices
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

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1, 
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*) 0
        );


        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); 
                         
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);


		// Swap buffers
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

