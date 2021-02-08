#include "VulkanModel.h"
#include <SDL_stdinc.h>
#include <iostream>
#include "glmWithPhysX.h"

std::vector<glm::vec3> lPositions = {
	glm::vec3(-4.0f,  4.0f, 4.0f),
	glm::vec3(4.0f,  4.0f, 4.0f),
	glm::vec3(-4.0f, -4.0f, 4.0f),
	glm::vec3(4.0f, -4.0f, 4.0f),
};

std::vector<glm::vec3> lColours = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};

//CLEAN UP WHEN FUNCTIONAL
void VulkanModel::update() {
	if (physicsObject) {
		//setPosition(glm::vec3(physicsObject->getGlobalPose().p.x, physicsObject->getGlobalPose().p.y, physicsObject->getGlobalPose().p.z)); //Try to pass PxReal everywhere to avoid weird floating point errors TODO
		//setRotation(glm::quat(physicsObject->getGlobalPose().q.w, physicsObject->getGlobalPose().q.x, physicsObject->getGlobalPose().q.y, physicsObject->getGlobalPose().q.z));
		worldTransform = pMat4ToGMat4(physx::PxMat44(physicsObject->getGlobalPose()));
		//float angle;
		//physx::PxVec3 axis;

		//physicsObject->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);
		//setRotation(angle, glm::vec3(axis.x, axis.y, axis.z)); //Bottom object is getting hit slightly, slightly changes rotation, this gets interpreted as angular velocity and keeps rotating, fix
		//If current object rotation != physX object rotation, rotate until they are equal then stop
	}
}

glm::vec3 VulkanModel::getPosition() {
	return glm::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
}

void VulkanModel::setPosition(glm::vec3 pos) {
	worldTransform[3][0] = pos.x;
	worldTransform[3][1] = pos.y;
	worldTransform[3][2] = pos.z;
}

glm::quat VulkanModel::getRotation() {
	return glm::quat_cast(worldTransform) * glm::quat_cast(localTransform);
}

void VulkanModel::setRotation(glm::quat qRot) {
	//Try setting rotation manually :: quat to mat4
	//worldTransform = glm::rotate(worldTransform, angle * 0.001f, axis); //Fix up 0.01 with tolerance scale
	worldTransform = worldTransform * glm::toMat4(qRot);
}

glm::mat4 VulkanModel::getTransform() {
	return worldTransform;
}

void VulkanModel::setTransform(glm::mat4 transform) {
	//worldTransform = transform; //mad lad
}

void VulkanModel::rotate(glm::vec3 rotateVec, float angle) { //break the vector into x and z rotation on local, and y on world
	//glm::vec3 local = glm::vec3(rotateVec.x, 0, rotateVec.z);
	//glm::vec3 world = glm::vec3(0, rotateVec.y, 0);

	//localTransform = glm::rotate(localTransform, angle, local);
	//worldTransform = glm::rotate(worldTransform, angle, rotateVec);
}

void VulkanModel::translate(glm::vec3 translateVec) {
	worldTransform = glm::translate(worldTransform, translateVec);
}

std::string VulkanModel::getModelLocation() {
	return modelLoc;
}

VkBuffer& VulkanModel::getVertexBuffer() {
	return vertexBuffer;
}

VkBuffer& VulkanModel::getIndexBuffer() {
	return indexBuffer;
}

PxRigidDynamic& VulkanModel::getRigidDynamicActor() {
	return *physicsObject;
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


void VulkanModel::loadModel(glm::vec3& cameraPos) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;


	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelLoc.c_str())) {
		throw std::runtime_error(warn + err);
	}

	materialLayers = materials;

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

			/* Lighting vertex buffer init stuff*/
			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertex.cameraPos = cameraPos; //How are we gonna inform this position of the camera's position?

			vertex.lightPos1 = lPositions[0];
			vertex.lightPos2 = lPositions[1];

			vertex.lightColour1 = lColours[0];
			vertex.lightColour2 = lColours[1];

			/*                                  */

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

VkImageView VulkanModel::getImageView(size_t idx) { //focus
	return textureStructs[idx].textureImageView;
}

VkSampler VulkanModel::getTextureSampler(size_t idx) {
	return textureStructs[idx].textureSampler;
}

glm::vec3 VulkanModel::getPhysXAABB() {
	//This function will get half extents of an AABB by going through all the verts
	//Verts >= 4
	float maxX = vertices[0].pos.x,
		maxY = vertices[0].pos.y,
		maxZ = vertices[0].pos.z,
		minX = vertices[1].pos.x,
		minY = vertices[1].pos.y,
		minZ = vertices[1].pos.z; //Init min and max variables to 1st and 2nd vertices, possible because v >= 4

	for (const auto vert : vertices) {
		//Go through all vertices, find out vertex value for when each co-ord (x, y, z) is at their maximum or minimum.
		if (vert.pos.x > maxX) {
			maxX = vert.pos.x;
		}
		else if (vert.pos.x < minX) {
			minX = vert.pos.x;
		}

		if (vert.pos.y > maxY) {
			maxY = vert.pos.y;
		}
		else if (vert.pos.y < minY) {
			minY = vert.pos.y;
		}

		if (vert.pos.z > maxZ) {
			maxZ = vert.pos.z;
		}
		else if (vert.pos.z < minZ) {
			minZ = vert.pos.z;
		}
	}

	glm::vec3 AABB(maxX - minX, maxY - minY, maxZ - minZ);
	AABB /= 2; //For half extents wanted by PhysX TWEAK THIS

	return AABB;
}

std::vector<Vertex>& VulkanModel::getVertices() {
	return vertices;
}

tinyobj::material_t& VulkanModel::getFirstMaterial() {
	return materialLayers[0];
}