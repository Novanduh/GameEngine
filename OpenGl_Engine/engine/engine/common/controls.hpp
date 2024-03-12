//From https://www.opengl-tutorial.org/download/
#ifndef CONTROLS_HPP
#define CONTROLS_HPP

vec3 getPlayerPos();
void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif