#include "Model.h"

Model::Model(const std::string modelLocNew, std::optional<glm::mat4> transformNew) {
	modelLoc = modelLocNew;
	if (!transformNew.has_value()) {
		transform = glm::mat4(1.0f);
	}
	else {
		transform = glm::mat4(1.0f);
	}

	//transform = glm::translate(transform, glm::vec3(0.1f));
	transform = glm::scale(transform, glm::vec3(1.0f));
}

glm::vec3 Model::getPosition() {
	return glm::vec3(transform[3][0], transform[3][1], transform[3][2]);
}

glm::mat4 Model::getRotation() {
	return glm::mat4_cast(rotation);
}

glm::mat4 Model::getTransform() {
	return transform;
}

float Model::getUniformScale() {
	return transform[3][3];
}

void Model::pushVertex(Vertex vertex) {
	modelVertices.push_back(vertex);
}

void Model::pushIndex(uint32_t index) {
	modelIndices.push_back(index);
}

int Model::getModelIndicesSize() {
	return modelIndices.size();
}

std::vector<Vertex>* Model::getModelVertices()
{
	return &modelVertices;
}

std::vector<uint32_t>* Model::getModelIndices()
{
	return &modelIndices;
}

std::string Model::getModelLoc() {
	return modelLoc;
}

void Model::moveUp() {
	transform[3][2] += 1;
}

void Model::moveDown() {
	transform[3][3] -= 1;
}