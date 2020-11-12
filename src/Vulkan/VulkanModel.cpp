#include "VulkanModel.h"
#include <SDL_stdinc.h>

void VulkanModel::update() {
	//rotate(glm::vec3(-0.5, 0, -1), M_PI / 360);
}

glm::vec3 VulkanModel::getPosition() {
	return glm::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
}

glm::quat VulkanModel::getRotation() {
	return glm::quat_cast(worldTransform) * glm::quat_cast(localTransform);
}

glm::mat4 VulkanModel::getTransform() {
	return worldTransform;
}

void VulkanModel::rotate(glm::vec3 rotateVec, float angle) { //break the vector into x and z rotation on local, and y on world
	//glm::vec3 local = glm::vec3(rotateVec.x, 0, rotateVec.z);
	//glm::vec3 world = glm::vec3(0, rotateVec.y, 0);

	//localTransform = glm::rotate(localTransform, angle, local);
	worldTransform = glm::rotate(worldTransform, angle, rotateVec);
}

void VulkanModel::translate(glm::vec3 translateVec) {
	worldTransform = glm::translate(worldTransform, translateVec);
}

std::string VulkanModel::getModelLocation() {
	return modelLoc;
}

std::string VulkanModel::getTextureLocation() {
	return texturePath;
}


VkBuffer& VulkanModel::getVertexBuffer() {
	return vertexBuffer;
}

VkBuffer& VulkanModel::getIndexBuffer() {
	return indexBuffer;
}






int VulkanModel::getModelIndicesSize() {
	return modelIndicesSize;
}

int VulkanModel::getModelVerticesSize() {
	return modelVerticesSize;
}

void VulkanModel::moveUp() {
	worldTransform[3][2] += 1;
}

void VulkanModel::moveDown() {
	worldTransform[3][3] -= 1;
}


void VulkanModel::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelLoc.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);

			//vertices.push_back(vertex);
			//indices.push_back(model.getModelIndicesSize());
		}
	}

	setupBuffers(vertices, indices);
}

VkImageView VulkanModel::getImageView() { //focus
	return textureImageView;
}