#include "Main.h"


int main(int argc, char* argv[])
{
	VulkanImpl vulkan;
	SoundManager soundManager;
	//Player player;
	Camera camera;

	try {
		//std::thread soundThr(&SoundManager::start, &soundManager);
		//std::thread vulkanThr(&VulkanImpl::run, &vulkan);
		vulkan.run();
		soundManager.start();
		camera.setup();
		//player.init();
		
		
		//soundThr.join();
		//vulkanThr.join();

		gameLoop(&vulkan, &soundManager, &camera);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void gameLoop(VulkanImpl* vulkan, SoundManager* soundManager, Camera* camera) {
	SDL_Event event;
	bool active = true;
	bool notN = false;

	bool firstLoop = true;

	float timestep = 1000.f / 144.0f;
	Uint32 timeLast = 0;
	Uint32 timeCurrent = 0;
	float deltaTime = 0.0f;
	float accumulatedTime = 0;

	float speed = 20.0f;

	float mouseSensitivity = 7.5f;

	while (active) {
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

		while (accumulatedTime >= timestep) {
			while (SDL_PollEvent(&event)) {
				camera->updateTick();
				SDL_PumpEvents();
				const Uint8* keystate = SDL_GetKeyboardState(NULL);

				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						active = false;

					if (event.key.keysym.sym == SDLK_UP) 
						vulkan->models[0].moveUp();

					if (event.key.keysym.sym == SDLK_s) {
						camera->translate(glm::vec3(deltaTime, 0.0f, 0.0f));
						//player->move(glm::vec3(deltaTime, 0.0f, 0.0f));
					}

					if (event.key.keysym.sym == SDLK_w) {
						//camera->translate(glm::vec3(-deltaTime, 0.0f, 0.0f));
						camera->moveInCameraDir(glm::vec3(-deltaTime, 0.0f, 0.0f), deltaTime);
						//player->move(glm::vec3(-deltaTime, 0.0f, 0.0f));
					}

					if (event.key.keysym.sym == SDLK_a) {
						//player->move(glm::vec3(0.0f, -deltaTime, 0.0f));
						camera->translate(glm::vec3(0.0f, -deltaTime, 0.0f));
					}

					if (event.key.keysym.sym == SDLK_d) {
						camera->translate(glm::vec3(0.0f, deltaTime, 0.0f));
						//player->move(glm::vec3(0.0f, deltaTime, 0.0f));
					}

					break;
				case SDL_QUIT:
					active = false;
					break;
				case SDL_WINDOWEVENT && SDL_WINDOWEVENT_RESIZED:
					vulkan->framebufferResizedSwitchPublic();
					break;
				case SDL_MOUSEMOTION:

					if (event.motion.xrel != 0) {
						camera->rotate(glm::vec3(0, 1, 0), mouseSensitivity * deltaTime * (event.motion.xrel > 0 ? 1 : -1), true);
						//player->addQuatRotationWorld(glm::vec3(0, 0, 1), mouseSensitivity * deltaTime * (event.motion.xrel > 0 ? 1 : -1));
					}

					if (event.motion.yrel != 0) {
						camera->rotate(glm::vec3(1, 0, 0), mouseSensitivity * deltaTime * (event.motion.yrel > 0 ? 1 : -1), false); // changing z to 1 instead locks camera to circle
						//player->addQuatRotationLocal(glm::vec3(1, 0, 0), mouseSensitivity * deltaTime * (event.motion.yrel > 0 ? -1 : 1));
					}

					
				
					break;
				}

				

			}
			accumulatedTime -= timestep;
		}

		vulkan->drawFramePublic(camera->getViewMatrix()); //change to getViewMatrix
	}

	vulkan->cleanupPublic();
	soundManager->end();
}