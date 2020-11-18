#pragma once
#include "Physics.h"
#include "PhysicsScene.h"
#include "Sound.h"
#include <glm/gtx/quaternion.hpp>
#include "Camera.h"
#include "Vulkan\VulkanModel.h"
#include <thread>

#include "Vulkan\VulkanImpl.h"

void gameLoop(VulkanImpl* vulkan, SoundManager* soundManager, Camera* camera, PhysicsScene* pScene);