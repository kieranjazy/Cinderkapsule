#pragma once
#include "vulkan/vulkan.h"
#include <string>

VkImage createTextureImage(const std::string texturePath, VkDevice& device, VkPhysicalDevice& physicalDevice, VkDeviceMemory& textureImageMemory, VkCommandPool& commandPool, VkQueue& graphicsQueue);
void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkDevice& device, VkPhysicalDevice& physicalDevice);
void transitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool& commandPool, VkDevice& device, VkQueue& graphicsQueue);
void copyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height, VkCommandPool& commandPool, VkDevice& device, VkQueue& graphicsQueue);
VkImageView createImageView(VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice& device);
VkImageView createTextureImageView(VkImage& image, VkDevice& device);