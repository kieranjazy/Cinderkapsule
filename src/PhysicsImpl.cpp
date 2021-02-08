#include "Physics.h"
#include <stdexcept>
#include "PhysicsScene.h"
#include <memory>
using namespace physx;






struct PhysicsImpl::impl {
	~impl() = default;

	PxFoundation* foundation;

	PxPvd* pvd;

	PxPhysics* physics;
	PxCooking* cooking;

	std::unique_ptr<PhysicsScene> currentScene; //IMPLEMENT

	PxTolerancesScale getPxTolerancesScale() {
		return physics->getTolerancesScale();
	}

	void setCurrentScene() {
		currentScene = std::make_unique<PhysicsScene>();
		currentScene->testInit();
	}

};

PhysicsImpl::PhysicsImpl() : pImpl(new impl()) {
}

PhysicsImpl::~PhysicsImpl() {
	release();
}

void PhysicsImpl::init() {



	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	pImpl->foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	if (!pImpl->foundation)
		throw std::runtime_error("Foundation create failed!");

	bool recordMemoryAllocations = true;

	pImpl->pvd = PxCreatePvd(*(pImpl->foundation));
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pImpl->pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	PxTolerancesScale scale;
	//scale.length = 100;
	scale.length = 1;
	scale.speed = 981;

	pImpl->physics = PxCreatePhysics(PX_PHYSICS_VERSION, *pImpl->foundation, scale, recordMemoryAllocations, pImpl->pvd);
	//pImpl->physics = PxCreatePhysics(PX_PHYSICS_VERSION, *pImpl->foundation, scale, recordMemoryAllocations, nullptr);

	if (!pImpl->physics) 
		throw std::runtime_error("Physics create failed!");

	pImpl->cooking = PxCreateCooking(PX_PHYSICS_VERSION, *pImpl->foundation, PxCookingParams(scale));

	if (!pImpl->cooking) 
		throw std::runtime_error("Cooking create failed!");

	if (!PxInitExtensions(*pImpl->physics, pImpl->pvd)) {
		throw std::runtime_error("Init extensions failed!");
	}


	pImpl->setCurrentScene();

}

void PhysicsImpl::startScene() {
	pImpl->setCurrentScene();
}


void PhysicsImpl::release() {

	PxCloseExtensions();
	pImpl->pvd->release();

	pImpl->currentScene->release();
	//pImpl->cooking->release();
	

	//pImpl->physics->release();
	pImpl->foundation->release();
	
	
	//pImpl->physics->release();
}

PhysicsScene* PhysicsImpl::getPhysicsScene() {
	return pImpl->currentScene.get();
}

