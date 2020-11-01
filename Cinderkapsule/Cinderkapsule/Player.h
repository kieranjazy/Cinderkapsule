#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/orthonormalize.hpp>

#include <chrono>

class Player {
public:
	void init() {
		setup();
	}


	void move(glm::vec3 moveVector) {
		translate(moveVector);
	}

	glm::mat4 getFacing() {
		return glm::lookAt(position, glm::vec3(position[0] + 1, position[1] + 1, position[2]), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::quat getQuatRotation() {
		return quatRotationWorld * quatRotationLocal;
	}

	void addQuatRotationLocal(glm::vec3 vec, float angle) {
		quatRotationLocal = glm::rotate(quatRotationLocal, angle, vec);
	}

	void addQuatRotationWorld(glm::vec3 vec, float angle) {
		quatRotationWorld = glm::rotate(quatRotationWorld, angle, vec);
	}
	glm::mat4 getViewMatrix() {
		return glm::mat4_cast(getQuatRotation()) * getFacing();
	}

	glm::vec3 getPosition() {
		return position;
	}

private:
	glm::vec3 position;
	glm::quat quatRotationLocal;
	glm::quat quatRotationWorld;

	void setup() {
		position = glm::vec3(2.0f, 2.0f, 2.0f);
		quatRotationLocal = glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		quatRotationWorld = glm::quat_cast(getFacing());
	}

	void translate(glm::vec3 moveVector) {
		position += moveVector;
	}
};
