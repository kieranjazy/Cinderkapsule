#include "CinderObject.h"

CinderObject::CinderObject() {

}

CinderObject::CinderObject(VulkanModel* vModel, PxRigidDynamic* pxActor) : vkModel(vModel), pActor(pxActor) {
	
}

void CinderObject::update() {
	glm::mat4 temp(0.0f); //transform and rotate by pactor getglobalpose
	temp = glm::translate(temp, glm::vec3(pActor->getGlobalPose().p.x, pActor->getGlobalPose().p.y, pActor->getGlobalPose().p.z));
	temp = glm::rotate(temp, pActor->getGlobalPose().q.w, glm::vec3(pActor->getGlobalPose().q.x, pActor->getGlobalPose().q.y, pActor->getGlobalPose().q.z));

	vkModel->setTransform(temp);
	//vkModel->setTransform(pActor->getGlobalPose());
}


















