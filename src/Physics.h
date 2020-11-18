#pragma once
#include "PxPhysicsAPI.h"
#include <memory>

class PhysicsImpl {	
public:
	PhysicsImpl();
	~PhysicsImpl();

	friend class PhysicsScene;

	void init();
	void release();

	physx::PxTolerancesScale getTScale();
	physx::PxScene* createScene(physx::PxSceneDesc sd);
	physx::PxMaterial* createMaterial(float a, float b, float c);

private:
	struct impl;
	std::unique_ptr<impl> pImpl;
};


