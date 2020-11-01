/*
#include "VulkanModel.h"

glm::vec3 VulkanModel::getPosition() {
	return glm::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
}

glm::quat VulkanModel::getRotation() {
	return glm::quat_cast(worldTransform) * glm::quat_cast(localTransform);
}

void VulkanModel::rotate(glm::vec3 rotateVec, float angle) { //break the vector into x and z rotation on local, and y on world
	glm::vec3 local = glm::vec3(rotateVec.x, 0, rotateVec.y);
	glm::vec3 world = glm::vec3(0, rotateVec.y, 0);

	localTransform = glm::rotate(localTransform, angle, local);
	worldTransform = glm::rotate(worldTransform, angle, world);
}

void VulkanModel::translate(glm::vec3 translateVec) {
	worldTransform = glm::translate(worldTransform, translateVec);
}

*/