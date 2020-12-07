#pragma once
#include "PxPhysicsAPI.h"
#include "PhysicsScene.h"
#include <memory>

class PhysicsImpl {	
public:
	PhysicsImpl();
	~PhysicsImpl();

	friend class PhysicsScene;

	void init();
	void release();

	physx::PxTolerancesScale getTScale();
	physx::PxMaterial* createMaterial(float a, float b, float c);

	void startScene();
	PhysicsScene* getPhysicsScene();

private:
	struct impl;
	std::unique_ptr<impl> pImpl;
};


