#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "Vertex.h"
#include <string>
#include "VulkanBuffer.h"
#include "VulkanTexture.h"
#include "PxPhysicsAPI.h"

#include <tiny_obj_loader.h>

#include <memory>
#include <stdexcept>
#include "common/PxBase.h"
#include "PxRigidActor.h"

using namespace physx;


class VulkanModel { //could I have just used inheritance from VulkanImpl...
public:
	glm::vec3 getPosition();
	glm::quat getRotation();

	void setPosition(glm::vec3 pos);
	void setTransform(glm::mat4 transform);

	void update();
	void rotate(glm::vec3 rotateVec, float angle);
	void translate(glm::vec3 translateVec);

	std::string getModelLocation();
	std::string getTextureLocation();

	glm::mat4 getTransform();
	int getModelIndicesSize();
	int getModelVerticesSize();
	VkImageView getImageView(); //Change to return reference
	VkSampler getTextureSampler();

	VkBuffer& getVertexBuffer();
	VkBuffer& getIndexBuffer();

	PxRigidDynamic& getRigidDynamicActor();

	void moveUp();
	void moveDown();

	void loadModel();

	VulkanModel(const std::string modelLocation, const std::string tex, VkDevice& dev, VkQueue& graphics, VkCommandPool& comPool, VkPhysicalDevice& physicalDev) : 
	modelLoc(modelLocation), texturePath(tex), device(&dev), graphicsQueue(&graphics), commandPool(&comPool), physicalDevice(&physicalDev){
		worldTransform = glm::mat4(1.0f);
		localTransform = glm::mat4(0.0f);

		//physicsObject = PxActor
		//physicsShape = PxRigidActorExt::createExclusiveShape(*physicsObject, PxBoxGeometry(0.125f, 0.125f, 0.125f), *physicsMaterial);
		//physicsObject->attachShape(*physicsShape);
		//physicsObject->setMass(1.0f);
		
		//PxRigidBodyExt::updateMassAndInertia(*physicsObject, 1.0f);
		//physicsObject = PxGetPhysics().createRigidDynamic()

		
		//setupTextures();

	}

	void updateCustomRigidDynamic(float hx, float hy, float hz) {
		physicsObject->detachShape(*physicsShape);
		physicsShape = PxRigidActorExt::createExclusiveShape(*physicsObject, PxBoxGeometry(hx, hy, hz), *physicsMaterial);
		physicsObject->attachShape(*physicsShape);
	}

	void setupPhysicsObject() {
		physicsMaterial = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.0f);
		physicsObject = PxGetPhysics().createRigidDynamic(PxTransform(PxVec3(getPosition()[0], getPosition()[1], getPosition()[2]))); //Setting this BEFORE we set position of objects doink!!!
	}

	//~VulkanModel() {}

	void destruct() {
		vkDestroyBuffer(*device, vertexBuffer, nullptr);
		vkFreeMemory(*device, vertexBufferMemory, nullptr);

		vkDestroyBuffer(*device, indexBuffer, nullptr);
		vkFreeMemory(*device, indexBufferMemory, nullptr);

		vkDestroySampler(*device, textureSampler, nullptr);

		vkDestroyImage(*device, texture, nullptr);

		vkDestroyImageView(*device, textureImageView, nullptr);

		vkFreeMemory(*device, textureImageMemory, nullptr);
		
		//Destroy the buffers created MAKE SURE WE DESTROY EVERYTHING RELATED TO THIS MODEL
	}


private:
	bool isLoaded; //setup
	std::string modelLoc;
	std::string texturePath;

	VkDevice* device;
	VkQueue* graphicsQueue;
	VkCommandPool* commandPool;
	VkPhysicalDevice* physicalDevice;

	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	VkDescriptorSet descriptorSet;

	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;


	VkDeviceMemory textureImageMemory;

	VkImage texture;
	VkImageView textureImageView;

	VkSampler textureSampler;
	//Add vulkan texture sampler per model

	glm::mat4 localTransform;
	glm::mat4 worldTransform;

	uint32_t modelIndicesSize, modelVerticesSize;

	PxRigidDynamic* physicsObject;
	PxShape* physicsShape;
	PxMaterial* physicsMaterial;

	void setupBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) {
		modelIndicesSize = static_cast<uint32_t>(indices.size());
		modelVerticesSize = static_cast<uint32_t>(verts.size());

		createVertexBuffer(verts);
		createIndexBuffer(indices);

		setupTextures();
	}

	
	void setupTextures() {
		texture = createTextureImage(texturePath, *device, *physicalDevice, textureImageMemory, *commandPool, *graphicsQueue);
		textureImageView = createTextureImageView(texture, *device);
		textureSampler = createTextureSampler(*device);
	}
	


	/*
	void setupDescriptorSets(int swapchainImagesSize, int modelsSize, int size, std::vector<VkBuffer>& uniformBuffers, VkSampler& sampler, VkDevice& device) {
		for (int i = 0; i != swapchainImagesSize; i++) {
			for (int j = 0; j != modelsSize; j++) {
				std::vector<VkWriteDescriptorSet> descWrites;
				descWrites.resize(2);

				VkDescriptorBufferInfo bufferInfo{};
				bufferInfo.offset = 0;
				bufferInfo.range = sizeof(UniformBufferObject);
				//bufferInfo.buffer = uniformBuffers[i];
				bufferInfo.buffer = uniformBuffers[j + (i * (modelsSize - 1))];

				VkDescriptorImageInfo imageInfo{};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = getImageView(); //set these up CHANGE TO model.textureImageView when using VulkanModel
				imageInfo.sampler = sampler; //set these up

				descWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descWrites[0].dstSet = descriptorSet;
				descWrites[0].dstBinding = 0;
				descWrites[0].dstArrayElement = 0;
				descWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descWrites[0].descriptorCount = 1;
				descWrites[0].pBufferInfo = &bufferInfo;

				descWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descWrites[1].dstSet = descriptorSet;
				descWrites[1].dstBinding = 1;
				descWrites[1].dstArrayElement = 0;
				descWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descWrites[1].descriptorCount = 1;
				descWrites[1].pImageInfo = &imageInfo;

				vkUpdateDescriptorSets(device, static_cast<uint32_t>(descWrites.size()), descWrites.data(), 0, nullptr);

			}
		}
	}
	*/

	void createVertexBuffer(std::vector<Vertex>& verts) {
		VkDeviceSize bufferSize = verts.size() * sizeof(Vertex);

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, *device, *physicalDevice);

		void* data;
		vulkanMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, verts.data(), (size_t)bufferSize);
		vulkanUnmapMemory(*device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, *device, *physicalDevice);
		copyBuffer(stagingBuffer, vertexBuffer, bufferSize, *commandPool, *device, *graphicsQueue);

		vulkanDestroyBuffer(*device, stagingBuffer);
		vulkanFreeMemory(*device, stagingBufferMemory);
	}


	void createIndexBuffer(std::vector<uint32_t>& indices) {
		VkDeviceSize bufferSize = indices.size() * sizeof(uint32_t);

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, *device, *physicalDevice);

		void* data;
		vulkanMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vulkanUnmapMemory(*device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, *device, *physicalDevice);
		copyBuffer(stagingBuffer, indexBuffer, bufferSize, *commandPool, *device, *graphicsQueue);

		vulkanDestroyBuffer(*device, stagingBuffer);
		vulkanFreeMemory(*device, stagingBufferMemory);
	}
};