#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>



class Camera {
public:
	void setup() {
		init();
	}

	void updateTick() {
		update();
	}


	glm::mat4 getViewMatrix() {
		return glm::toMat4(glm::toQuat(worldTransform) * glm::toQuat(localTransform)) * glm::lookAt(glm::vec3(worldTransform[3]), getCameraFacing(), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::vec3 getCameraFacing() {
		//return glm::vec3(worldTransform[3][0] + 1, worldTransform[3][1], worldTransform[3][2] + 1); //rotate this by local transform and world transform rotations
		glm::vec3 temp(worldTransform[3][0] + 1, worldTransform[3][1] + 1, worldTransform[3][2]);
		//glm::quat rotateLocal = glm::toQuat(localTransform);
		//glm::quat rotateWorld = glm::toQuat(worldTransform);
		//temp = glm::rotate(rotateLocal, temp);
		//temp = glm::rotate(rotateWorld, temp);
		//temp = glm::rotate(rotateWorld * rotateLocal, temp);
		return temp;
	}

	void translate(glm::vec3 transVec) {
		transVec *= 2;
		worldTransform = glm::translate(worldTransform, transVec);
	}

	void rotate(glm::vec3 axis, float angle, bool local) { //Remember to stop pitch from being equal to 90.0f
		if (local) {
			localTransform = glm::rotate(localTransform, angle, axis);
		}
		else {
			//if (rotation.w > 0.99f)  ///put pitch stuff here
				//rotation.w = 0.99f;

			worldTransform = glm::rotate(worldTransform, angle, axis);
		}
	}

	void moveInCameraDir(glm::vec3 dir, float dt) {
		glm::mat4 tempView = glm::inverse(getViewMatrix());
		glm::vec3 forward(-tempView[2][0], -tempView[2][1], -tempView[2][2]);
		//forward = glm::rotate(forward, glm::vec3(0, 0, 1));
		//forward = glm::normalize(forward);

		std::cout << "CAMFORWARD X: " << forward.x << " Y: " << forward.y << " Z: " << forward.z << std::endl;
		forward *= dt;
		forward *= 3;
		worldTransform = glm::translate(worldTransform, forward);
		//worldTransform = glm::translate(worldTransform, );
		//worldTransform = glm::translate(getViewMatrix(), dir); // ????????????????????????????????????????
		//tempQuat holds the rotation of the actual camera; We need the front vector for it
	}


private:
	glm::mat4 localTransform;
	glm::mat4 worldTransform;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	glm::vec3 skew;
	glm::vec4 perspective;

	

	void init() {
		localTransform = glm::mat4(1.0f);
		//localTransform = glm::rotate(localTransform, glm::radians(90.0f), glm::vec3(0, 1, 0));
		worldTransform = glm::mat4(1.0f);
		worldTransform = glm::translate(worldTransform, glm::vec3(-1.0f, -1.0f, 1.0f));
		glm::decompose(worldTransform, scale, rotation, position, skew, perspective);
	}

	void update() {
		glm::decompose(worldTransform, scale, rotation, position, skew, perspective);
		rotation = rotation * glm::toQuat(localTransform);
		std::cout << "W: " << rotation.w << std::endl;
	}



};