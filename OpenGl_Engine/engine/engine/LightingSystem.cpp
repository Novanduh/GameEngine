#include "LightingSystem.hpp"

void LightingSystem::addLight(Light light)
{
	lights.push_back(light);
}

void LightingSystem::calcLight()
{
	clearProducts();
	glm::vec3 ambient, diffuse, specular;
	for (int i = 0; i < lights.size(); i++) {
		ambient = material.getAmbient() * lights[i].getLightAmbient();
		diffuse = material.getDiffuse() * lights[i].getLightDiffuse();
		specular = material.getSpecular() * lights[i].getLightSpecular();
		ambientProduct.push_back(ambient);
		diffuseProduct.push_back(diffuse);
		specularProduct.push_back(specular);
	}
}

std::vector<glm::vec3> LightingSystem::getAmbientProduct()
{
	return ambientProduct;
}

std::vector<glm::vec3> LightingSystem::getDiffuseProduct()
{
	return diffuseProduct;
}

std::vector<glm::vec3> LightingSystem::getSpecularProduct()
{
	return specularProduct;
}

void LightingSystem::clearProducts()
{
	ambientProduct.clear();
	diffuseProduct.clear();
	specularProduct.clear();
}
