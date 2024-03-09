#include "LightingSystem.hpp"

void LightingSystem::addLight(Light light)
{
	lights.push_back(light);
}

void LightingSystem::calcLight()
{

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
