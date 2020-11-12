#pragma once
#include "vulkan/vulkan.h"

void vulkanMapMemory(VkDevice& device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** pdata);
void vulkanUnmapMemory(VkDevice& device, VkDeviceMemory memory);
void vulkanDestroyBuffer(VkDevice& device, VkBuffer buffer);
void vulkanFreeMemory(VkDevice& device, VkDeviceMemory bufferMemory);
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool& commandPool, VkDevice& device, VkQueue& graphicsQueue);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& device, VkPhysicalDevice& physicalDevice);
VkCommandBuffer beginSingleTimeCommands(VkCommandPool& commandPool, VkDevice& device);
void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue& graphicsQueue, VkDevice& device, VkCommandPool& commandPool);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);