#include "Physics.h"
#include <stdexcept>

using namespace physx;
//#define _DEBUG

struct PhysicsImpl::impl {
	~impl() = default;

	PxFoundation* foundation;
	PxPvd* pvd;
	PxPhysics* physics;
	PxCooking* cooking;

	PxScene* currentScene; //IMPLEMENT

	PxTolerancesScale getPxTolerancesScale() {
		return physics->getTolerancesScale();
	}

};

PhysicsImpl::PhysicsImpl() : pImpl(new impl()) {}
PhysicsImpl::~PhysicsImpl() = default;

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
	scale.length = 100;
	scale.speed = 981;

	pImpl->physics = PxCreatePhysics(PX_PHYSICS_VERSION, *pImpl->foundation, PxTolerancesScale(scale), recordMemoryAllocations, pImpl->pvd);

	if (!pImpl->physics) 
		throw std::runtime_error("Physics create failed!");

	pImpl->cooking = PxCreateCooking(PX_PHYSICS_VERSION, *pImpl->foundation, PxCookingParams(scale));

	if (!pImpl->cooking) 
		throw std::runtime_error("Cooking create failed!");

	if (!PxInitExtensions(*pImpl->physics, pImpl->pvd)) {
		throw std::runtime_error("Init extensions failed!");
	}

}

void PhysicsImpl::release() {
	PxCloseExtensions();
	pImpl->cooking->release();
	pImpl->physics->release();
	pImpl->pvd->release();
	pImpl->foundation->release();
}

