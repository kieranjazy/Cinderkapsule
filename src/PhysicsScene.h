#pragma once
#include "PxPhysicsAPI.h"
#include <memory>
#include "Physics.h"

using namespace physx;

class PhysicsScene{

public:
	void testInit();
	void addDefault();

	void release();

	PxScene* getScene();

	PhysicsScene();
	~PhysicsScene();

	void addRigidBody(PxRigidDynamic& rb);
private:
	struct impl;
	std::unique_ptr<impl> pImpl;

};







