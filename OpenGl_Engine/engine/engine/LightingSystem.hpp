#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <vector>
#include "Light.hpp"
using namespace glm;


class StandardMaterial {
private:
	float shininess = 60.0f;
	glm::vec3 materialAmbientValue = vec3(0.5, 0.2, 0.7);
	glm::vec3 materialDiffuseValue = vec3(0.7, 0.2, 0.7);
	glm::vec3 materialSpecularValue = vec3(0.5, 0.2, 0.5);

public:
	glm::vec3 getAmbient() {
		return materialAmbientValue;
	}
	glm::vec3 getDiffuse() {
		return materialDiffuseValue;
	}
	glm::vec3 getSpecular() {
		return materialSpecularValue;
	}
	float getShininess() {
		return shininess;
	}
};

class LightingSystem {
private:
	std::vector<glm::vec3> ambientProduct;
	std::vector<glm::vec3> diffuseProduct;
	std::vector<glm::vec3> specularProduct;
	std::vector<Light> lights;

public:
	StandardMaterial material;
	void addLight(Light light);
	void calcLight();
	std::vector<glm::vec3> getAmbientProduct();
	std::vector<glm::vec3> getDiffuseProduct();
	std::vector<glm::vec3> getSpecularProduct();
	void clearProducts();
};