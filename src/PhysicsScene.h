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
	void setPhysicsImpl(PhysicsImpl* p);

	PxScene* getScene();

	PhysicsScene();
	PhysicsScene(PhysicsImpl& p);
	~PhysicsScene();

	PhysicsImpl* physics = nullptr;
	void addRigidBody(PxRigidDynamic* rb);
private:
	struct impl;
	std::unique_ptr<impl> pImpl;

};







