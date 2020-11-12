#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//PImpl style attempt
struct Camera::impl {
	~impl() = default;

	glm::mat4 localTransform;
	glm::mat4 worldTransform;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	glm::vec3 skew;
	glm::vec4 perspective;

	void init() { //My default camera init for the moment
		localTransform = glm::mat4(1.0f);
		worldTransform = glm::mat4(1.0f);
		worldTransform = glm::translate(worldTransform, glm::vec3(-1.0f, -1.0f, 1.0f));
		glm::decompose(worldTransform, scale, rotation, position, skew, perspective);
	}

	void update() {
		glm::decompose(worldTransform, scale, rotation, position, skew, perspective);
		rotation = rotation * glm::toQuat(localTransform);
	}
};

Camera::Camera() : pImpl(new impl()) {}
Camera::~Camera() = default;

void Camera::setup() {
	pImpl->init();
}

void Camera::updateTick() {
	pImpl->update();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::toMat4(glm::toQuat(pImpl->worldTransform) * glm::toQuat(pImpl->localTransform)) * glm::lookAt(glm::vec3(pImpl->worldTransform[3]), getCameraFacing(), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::vec3 Camera::getCameraFacing() {
	glm::vec3 temp(pImpl->worldTransform[3][0] + 1, pImpl->worldTransform[3][1] + 1, pImpl->worldTransform[3][2]);
	return temp;
}

void Camera::translate(glm::vec3 transVec) {
	pImpl->worldTransform = glm::translate(pImpl->worldTransform, transVec);
}

void Camera::rotate(glm::vec3 axis, float angle, bool local) { //Remember to stop pitch from being equal to 90.0f
	if (local) {
		pImpl->localTransform = glm::rotate(pImpl->localTransform, angle, axis);
	}
	else {
		//if (rotation.w > 0.99f)  ///put pitch stuff here
			//rotation.w = 0.99f;

		pImpl->worldTransform = glm::rotate(pImpl->worldTransform, angle, axis);
	}
}

void Camera::moveInCameraDir(glm::vec3 dir, float dt) {
	glm::mat4 tempView = glm::inverse(getViewMatrix());
	glm::vec3 forward(-tempView[2][0], -tempView[2][1], -tempView[2][2]);
	forward *= dt;
	pImpl->worldTransform = glm::translate(pImpl->worldTransform, forward);
}