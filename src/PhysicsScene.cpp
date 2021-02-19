#include "PhysicsScene.h"
#include <stdexcept>
#include <vector>
#include "vulkan/VulkanModel.h"

struct PhysicsScene::impl {
	~impl() {
		delete cpuDispatcher;
		scene->release();
	}

	PxScene* scene;
	PxCpuDispatcher* cpuDispatcher;

	//std::vector<VulkanModel>& models;

	const PxU32 numThreads = 4;
};


PhysicsScene::PhysicsScene() : pImpl(new impl()) {
	testInit();
}

PhysicsScene::~PhysicsScene() {

	pImpl.release();
}

void PhysicsScene::testInit() {
	PxSceneDesc sd(PxGetPhysics().getTolerancesScale());
	sd.gravity = PxVec3(0.0f, 0.0f, -9.81f);

	pImpl->cpuDispatcher = PxDefaultCpuDispatcherCreate(pImpl->numThreads);
	if (!pImpl->cpuDispatcher)
		throw std::runtime_error("CPU dispatcher create failed!");

	sd.cpuDispatcher = pImpl->cpuDispatcher;
	sd.filterShader = PxDefaultSimulationFilterShader;
	//sd.cudaContextManager = gCuda

	pImpl->scene = PxGetPhysics().createScene(sd);
	pImpl->scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);
	 /*
	PxSphereGeometry geometry(1.0f);
	PxMaterial* material = physics->createMaterial(0.0f, 0.0f, 0.0f);
	PxShape* shape = PxGetPhysics().createShape(geometry, *material);
	PxTransform t = PxTransform(PxIdentity);
	PxRigidDynamic* dynamic = PxCreateDynamic(PxGetPhysics(), t, geometry, *material, 1.0f);

	PxSerializationRegistry* registry = PxSerialization::createSerializationRegistry(PxGetPhysics());

	PxCollection* collection = PxCreateCollection();
	collection->add(*dynamic);
	PxSerialization::complete(*collection, *registry);

	PxDefaultFileOutputStream outStream("serialized.dat");

	PxSerialization::serializeCollectionToBinary(outStream, *collection, *registry);
	*/
}

void PhysicsScene::addDefault() {
	/*
	PxSerializationRegistry* registry = PxSerialization::createSerializationRegistry(PxGetPhysics());

	FILE* fp;
	fopen_s(&fp, "serialized.dat", "rb");
	fseek(fp, 0, SEEK_END);
	unsigned fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	void* memory = malloc(fileSize + PX_SERIAL_FILE_ALIGN);
	void* memory128 = (void*)((size_t(memory) + PX_SERIAL_FILE_ALIGN) & ~(PX_SERIAL_FILE_ALIGN - 1));
	fread(memory128, 1, fileSize, fp);
	fclose(fp);
	PxCollection* collection = PxSerialization::createCollectionFromBinary(memory128, *registry);

	if (pImpl->scene)
		pImpl->scene->addCollection(*collection);
	*/
}

void PhysicsScene::release() {
	//pImpl->scene->release();

}

PxScene* PhysicsScene::getScene() {
	return pImpl->scene;
}

void PhysicsScene::addRigidBody(PxRigidDynamic& rb) {
		pImpl->scene->addActor(rb);
}