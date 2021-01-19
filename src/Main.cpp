#include "Main.h"

#ifndef NDEBUG
#define NDEBUG
#endif


int main(int argc, char* argv[])
{
	VulkanImpl vulkan;
	PhysicsImpl physics;
	SoundManager soundManager;
	Camera camera;

	try {
		//std::thread soundThr(&SoundManager::start, &soundManager);
		//std::thread vulkanThr(&VulkanImpl::run, &vulkan);
		physics.init();
		vulkan.run();
		soundManager.start();
		camera.setup();
		

		for (VulkanModel model : vulkan.models) {
			physics.getPhysicsScene()->addRigidBody(model.getRigidDynamicActor());
		}


		//pScene.testInit();
		//pScene.addDefault();
		//soundThr.join();
		//vulkanThr.join();

		gameLoop(&vulkan, &soundManager, &camera, &physics);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void gameLoop(VulkanImpl* vulkan, SoundManager* soundManager, Camera* camera, PhysicsImpl* physics) {
	SDL_Event event;




	//Time

	bool active = true;
	bool notN = false;
	bool firstLoop = true;

	float timestep = 1000.f / 144.0f;
	Uint32 timeLast = 0;
	Uint32 timeCurrent = 0;
	float deltaTime = 0.0f;
	float accumulatedTime = 0;

	//

	float speed = 20.0f;
	float mouseSensitivity = 30.0f;

	while (active) {

		//Time Logic
		if (!notN) {
			timeLast = SDL_GetTicks();
			notN = !notN;
			continue;
		}
		else {
			if (!firstLoop)
				timeLast = timeCurrent;

			timeCurrent = SDL_GetTicks();
			deltaTime = (timeCurrent - timeLast) / 1000.0f;
			accumulatedTime += deltaTime * 1000;
			firstLoop = false;
		}
		//

		while (accumulatedTime >= timestep) {
			for (size_t i = 0; i != vulkan->models.size(); i++) {
				vulkan->models[i].update();
			}

			physics->getPhysicsScene()->getScene()->simulate(deltaTime);
			physics->getPhysicsScene()->getScene()->fetchResults(true);

			

			while (SDL_PollEvent(&event) ) {
				camera->updateTick();
				handleInputs(camera, deltaTime);
				
				
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						active = false;

					if (event.key.keysym.sym == SDLK_s) 
						camera->translate(glm::vec3(deltaTime, 0.0f, 0.0f)); //Change

					if (event.key.keysym.sym == SDLK_w) 
						//camera->moveInCameraDir(glm::vec3(-1.0f, 0.0f, 0.0f), deltaTime * 30.0f);

					if (event.key.keysym.sym == SDLK_a) 
						camera->translate(glm::vec3(0.0f, -deltaTime, 0.0f)); //Change

					if (event.key.keysym.sym == SDLK_d) 
						camera->translate(glm::vec3(0.0f, deltaTime, 0.0f)); //Change

					if (event.key.keysym.sym == SDLK_t) {
						vulkan->models[0].getRigidDynamicActor().setLinearVelocity(PxVec3(-5.0f, 0.0f, 0.0f));
					}
					
					break;
				case SDL_QUIT:
					active = false;
					break;
				case SDL_WINDOWEVENT && SDL_WINDOWEVENT_RESIZED:
					vulkan->framebufferResizedSwitchPublic();
					break;
				case SDL_MOUSEMOTION:
					if (event.motion.xrel != 0) 
						camera->rotate(glm::vec3(0, 1, 0), mouseSensitivity * deltaTime * (event.motion.xrel > 0 ? 1 : -1), true);

					if (event.motion.yrel != 0) 
						camera->rotate(glm::vec3(1, 0, 0), mouseSensitivity * deltaTime * (event.motion.yrel > 0 ? 1 : -1), false); // changing z to 1 instead locks camera to circle

					break;
				}
			}

			accumulatedTime -= timestep;

		}

		vulkan->drawFramePublic(camera->getViewMatrix());
		
		/*
		for (VulkanModel& model : vulkan->models) {
			model.update();
		}
		*/

		
	}

	vulkan->cleanupPublic();
	soundManager->end();
	//pScene->release();
	//PxGetPhysics().release();
}

void handleInputs(Camera* camera, float deltaTime) {
	const Uint8* ks = SDL_GetKeyboardState(nullptr);
	
	if (ks[SDL_SCANCODE_W]) {
		camera->moveInCameraDir(glm::vec3(-1.0f, 0.0f, 0.0f), (1.f/144.f));
	}
}