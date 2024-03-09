#include "Light.hpp"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	lightPosition = position;
	lightAmbientValue = ambient;
	lightDiffuseValue = diffuse;
	lightSpecularValue = specular;
}

void Light::setLightPosition(glm::vec3 position)
{
	lightPosition = position;
}

void Light::setLightAmbient(glm::vec3 value)
{
	lightAmbientValue = value;
}

void Light::setLightDiffuse(glm::vec3 value)
{
	lightDiffuseValue = value;
}

void Light::setLightSpecular(glm::vec3 value)
{
	lightSpecularValue = value;
}

glm::vec3 Light::getLightPosition()
{
	return lightPosition;
}

glm::vec3 Light::getLightAmbient()
{
	return lightAmbientValue;
}

glm::vec3 Light::getLightDiffuse()
{
	return lightDiffuseValue;
}

glm::vec3 Light::getLightSpecular()
{
	return lightSpecularValue;
}
