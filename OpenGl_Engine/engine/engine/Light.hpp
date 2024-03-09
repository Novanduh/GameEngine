#ifndef LIGHT_H
#define LIGHT_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
using namespace glm;

class Light {
private:
	glm::vec3 lightPosition;
	glm::vec3 lightAmbientValue;
	glm::vec3 lightDiffuseValue;
	glm::vec3 lightSpecularValue;

public:
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void setLightPoistion(glm::vec3 position);
	void setLightAmbient(glm::vec3 value);
	void setLightDiffuse(glm::vec3 value);
	void setLightSpecular(glm::vec3 value);

	glm::vec3 getLightPoistion();
	glm::vec3 getLightAmbient();
	glm::vec3 getLightDiffuse();
	glm::vec3 getLightSpecular();
};
#endif