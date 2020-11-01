#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <string>
#include <optional>
#include <vector>
#include <iostream>
#include "vertex.h"

class Model {
public:
	void pushVertex(Vertex vertex);
	void pushIndex(uint32_t index);

	void moveUp();
	void moveDown();

	glm::vec3 getPosition();
	glm::mat4 getRotation();

	glm::mat4 getTransform();

	float getUniformScale();

	int getModelIndicesSize();

	std::vector<Vertex>* getModelVertices();
	std::vector<uint32_t>* getModelIndices();

	std::string getModelLoc();

	Model(const std::string modelLocNew, std::optional<glm::mat4> transformNew);

private:
	std::string modelLoc;
	glm::mat4 transform;
	glm::quat rotation;

	std::vector<Vertex> modelVertices;
	std::vector<uint32_t> modelIndices;

};
