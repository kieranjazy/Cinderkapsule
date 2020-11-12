#pragma once

#include "Sound.h"
#include <glm/gtx/quaternion.hpp>
#include "Camera.h"
#include <thread>

#include "Vulkan\VulkanImpl.h"

void gameLoop(VulkanImpl* vulkan, SoundManager* soundManager, Camera* camera);