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
	InputManager inputManager(vulkan, physics, camera);

	try {
		//std::thread soundThr(&SoundManager::start, &soundManager);
		//std::thread vulkanThr(&VulkanImpl::run, &vulkan);
		camera.setup();
		physics.init();
		vulkan.run();
		soundManager.start();
		
		

		for (VulkanModel model : vulkan.models) {
			physics.getPhysicsScene()->addRigidBody(model.getRigidDynamicActor());
		}


		//pScene.testInit();
		//pScene.addDefault();
		//soundThr.join();
		//vulkanThr.join();

		gameLoop(&vulkan, &soundManager, &camera, &physics, &inputManager);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void gameLoop(VulkanImpl* vulkan, SoundManager* soundManager, Camera* camera, PhysicsImpl* physics, InputManager* inputManager) {
	SDL_Event e;



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

	inputManager->setActivePtr(active);

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
			camera->updateTick();
			inputManager->update(e, deltaTime);
			accumulatedTime -= timestep;
		}

		vulkan->drawFramePublic(camera->getViewMatrix());

	}

	vulkan->cleanupPublic();
	soundManager->end();
	//pScene->release();
	//PxGetPhysics().release();
}

