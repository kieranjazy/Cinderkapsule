#pragma once
#include "VulkanImpl.h"
#include "Sound.h"
#include "Player.h"
#include <glm/gtx/quaternion.hpp>
#include "Camera.h"
#include <thread>

void gameLoop(VulkanImpl* vulkan, SoundManager* soundManager, Camera* camera);