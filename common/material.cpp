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
    
#include "material.hpp"

void addMaterial(Material* sceneHead){
    Material* newObject = new Material();

    Material* iter = sceneHead;
    int i = 0;
    while(iter->next != NULL){
        iter = iter->next;
        i++;
    }
    newObject->data = i;
    iter->next = newObject;
}

