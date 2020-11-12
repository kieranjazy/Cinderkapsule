#pragma once
#include <glm/glm.hpp>
#include <memory>

class Camera {
public:
	Camera();
	~Camera();

	void setup();
	void updateTick();

	glm::mat4 getViewMatrix();
	glm::vec3 getCameraFacing();

	void translate(glm::vec3 transVec);
	void rotate(glm::vec3 axis, float angle, bool local);
	void moveInCameraDir(glm::vec3 dir, float dt);
private:
	struct impl;
	std::unique_ptr<impl> pImpl;
};