#pragma once
#include "Physics.h"
#include "PhysicsScene.h"
#include "Sound.h"
#include <glm/gtx/quaternion.hpp>
#include "Camera.h"
#include "Vulkan\VulkanModel.h"
#include <thread>

#include "Vulkan\VulkanImpl.h"
#include <memory>



class InputManager { //gonna keep inputs low level to minimise delay
public:
	float speed = 20.0f;
	float mouseSensitivity = 35.0f;

	std::unique_ptr<VulkanImpl> vulkan;
	std::unique_ptr<PhysicsImpl> physics;
	std::unique_ptr<Camera> camera; //make everything unique_ptr
	std::unique_ptr<bool> isActive;

	InputManager(VulkanImpl& vulk, PhysicsImpl& phys, Camera& cam) : vulkan(&vulk), physics(&phys), camera(&cam), isActive(nullptr) {

	}

	void setActivePtr(bool& active) {
		isActive.reset(&active);
	}

	void update(SDL_Event &e, float deltaTime) {
		 SDL_PumpEvents();
		 const Uint8* state = SDL_GetKeyboardState(NULL);
		 
		 if (state[SDL_SCANCODE_W]) {
			 camera->moveInCameraDir(glm::vec3(1.0f, 0.0f, 0.0f), deltaTime * 30.0f);
		 }

		 if (state[SDL_SCANCODE_A]) {
			 camera->moveInCameraDir(glm::vec3(0.0f, -1.0f, 0.0f), deltaTime * 25.0f);
		 }

		 if (state[SDL_SCANCODE_S]) {
			 camera->moveInCameraDir(glm::vec3(-1.0f, 0.0f, 0.0f), deltaTime * 30.0f);
		 }

		 if (state[SDL_SCANCODE_D]) {
			 camera->moveInCameraDir(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime * 25.0f);
		 }

		 if (state[SDL_SCANCODE_ESCAPE]) {
			 *isActive = false;
		 }

		 if (state[SDL_SCANCODE_T]) {
			 vulkan->models[0].getRigidDynamicActor().setLinearVelocity(PxVec3(-5.0f, 0.0f, 0.0f));
		 }

		 //SDL_GetMouseState


		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				/*

				if (e.key.keysym.sym == SDLK_ESCAPE) {
					*isActive = false;
				}

				if (e.key.keysym.sym == SDLK_s) {
					camera->translate(glm::vec3(deltaTime, 0.0f, 0.0f)); //Change
				}

				if (e.key.keysym.sym == SDLK_w) {
					camera->moveInCameraDir(glm::vec3(-1.0f, 0.0f, 0.0f), deltaTime * 30.0f);
				}

				if (e.key.keysym.sym == SDLK_a) {
					camera->translate(glm::vec3(0.0f, -deltaTime, 0.0f)); //Change
				}

				if (e.key.keysym.sym == SDLK_d) {
					camera->translate(glm::vec3(0.0f, deltaTime, 0.0f)); //Change
				}

				if (e.key.keysym.sym == SDLK_t) {
					vulkan->models[0].getRigidDynamicActor().setLinearVelocity(PxVec3(-5.0f, 0.0f, 0.0f));
				}

				*/

				break;
			case SDL_QUIT:
				*isActive = false;
				break;
			case SDL_WINDOWEVENT&& SDL_WINDOWEVENT_RESIZED:
				vulkan->framebufferResizedSwitchPublic();
				break;
			case SDL_MOUSEMOTION:
				if (e.motion.xrel != 0)
					camera->rotate(glm::vec3(0, 1, 0), mouseSensitivity * deltaTime * (e.motion.xrel > 0 ? 1 : -1), true);

				if (e.motion.yrel != 0)
					camera->rotate(glm::vec3(1, 0, 0), mouseSensitivity * deltaTime * (e.motion.yrel > 0 ? 1 : -1), false); // changing z to 1 instead locks camera to circle

				break;
			}
		}

	}

	~InputManager() {
		//delete vulkan;
		//delete physics;
		//delete camera;
		//delete isActive;

		vulkan.release();
		physics.release();
		camera.release();
		isActive.release();
	}

};

void gameLoop(VulkanImpl* vulkan, SoundManager* soundManager, Camera* camera, PhysicsImpl* physics, InputManager* inputManager);