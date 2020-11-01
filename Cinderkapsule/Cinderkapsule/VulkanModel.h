#pragma once
#include <glm/glm.hpp> //umm not sure about this one fam maybe put them in same namespace or something
#include <vector>
#include "Vertex.h"
#include <string>
#include "VulkanImpl.h"

class VulkanModel {
public:
	void update();

	glm::vec3 getPosition();
	glm::quat getRotation();

	void rotate(glm::vec3 rotateVec, float angle);
	void translate(glm::vec3 translateVec);

	std::string& getModelLoc();
	std::string& getTextureLoc();


	VulkanModel(const std::string modelLocation, const std::string tex, std::vector<Vertex>& verts, std::vector<uint32_t>& indices, VulkanImpl* vulkan) : modelLoc(modelLocation), vulkanImpl(vulkan), texturePath(tex) {
		setupBuffers(verts, indices);
		setupTextures();
		//setupDescriptorSets();
	}

	~VulkanModel() {
		vkDestroyBuffer(vulkanImpl->device, vertexBuffer, nullptr);
		vkFreeMemory(vulkanImpl->device, vertexBufferMemory, nullptr);

		vkDestroyBuffer(vulkanImpl->device, indexBuffer, nullptr);
		vkFreeMemory(vulkanImpl->device, indexBufferMemory, nullptr);

		vkDestroyImage(vulkanImpl->device, texture, nullptr);
		vkDestroyImageView(vulkanImpl->device, textureImageView, nullptr);

		delete vulkanImpl;
	
	} //Destroy the buffers created MAKE SURE WE DESTROY EVERYTHING RELATED TO THIS MODEL

private:
	bool isLoaded; //setup
	std::string modelLoc;
	std::string texturePath;

	VulkanImpl* vulkanImpl;

	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	VkDescriptorSet descriptorSet;

	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;

	VkImage texture;
	VkImageView textureImageView;

	glm::mat4 localTransform;
	glm::mat4 worldTransform;

	void setupBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) {
		createLocalBuffer(verts, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		createLocalBuffer(indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	}

	void setupTextures() {
		texture = vulkanImpl->createTextureImage(texturePath);
		textureImageView = vulkanImpl->createTextureImageView(texture);
	}



	/*
	void setupDescriptorSets(int i, int j, int size, std::vector<VkBuffer>& uniformBuffers, VkSampler& sampler) {
		std::vector<VkWriteDescriptorSet> descWrites;
		descWrites.resize(2);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);
		bufferInfo.buffer = uniformBuffers[j + (i * (size - 1))];//implement

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = sampler;

	}
	*/

	template <typename T>
	void createLocalBuffer(std::vector<T>& vec, VkBufferUsageFlags flags) {
		VkDeviceSize bufferSize = vec.size() * sizeof(T);

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		vulkanImpl->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vulkanImpl->vulkanMapMemory(stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vec.data(), (size_t)bufferSize);
		vulkanImpl->vulkanUnmapMemory(stagingBufferMemory);

		

		if (flags == VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {
			vulkanImpl->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | flags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
			vulkanImpl->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		}
		else {
			vulkanImpl->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | flags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
			vulkanImpl->copyBuffer(stagingBuffer, indexBuffer, bufferSize);
		}

		vulkanImpl->vulkanDestroyBuffer(stagingBuffer);
		vulkanImpl->vulkanFreeMemory(stagingBufferMemory);
	}
};