#include "Player.h"
#include "../Protocol.h"
#include <Platform/Platform.h>
#include <Utilities/Input.h>
namespace Minecraft::Internal {
	Player::Player()
	{
		onGround = true;
		x = y = z = 0.0;
		yaw = pitch = 0.0;
		timer.reset();
		positionChanged = false;
		rotationChanged = false;

#if CURRENT_PLATFORM != PLATFORM_PSP
		glfwSetInputMode(Platform::PC::g_Window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif
	}

	Player::~Player()
	{
#if CURRENT_PLATFORM != PLATFORM_PSP
		glfwSetInputMode(Platform::PC::g_Window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif
	}

	void Player::update()
	{
		float dt = timer.deltaTime();
		//Update based on player controls

		//State -1 = no move
		//State 0 = forward
		//State 1 = forward left
		//State 2 = left
		//State 3 = backward left
		//State 4 = backward
		//State 5 = backward right
		//State 6 = right
		//State 7 = forward right

		int run_state = -1;
		if ( (Utilities::KeyPressed(GLFW_KEY_W) || Utilities::KeyHold(GLFW_KEY_W) || Utilities::KeyPressed(PSP_CTRL_UP) || Utilities::KeyHold(PSP_CTRL_UP)) && !(Utilities::KeyPressed(GLFW_KEY_S) || Utilities::KeyHold(GLFW_KEY_S) || Utilities::KeyPressed(PSP_CTRL_DOWN) || Utilities::KeyHold(PSP_CTRL_DOWN))) {
			if ((Utilities::KeyPressed(GLFW_KEY_A) || Utilities::KeyHold(GLFW_KEY_A) || Utilities::KeyPressed(PSP_CTRL_LEFT) || Utilities::KeyHold(PSP_CTRL_LEFT)) && !(Utilities::KeyPressed(GLFW_KEY_D) || Utilities::KeyHold(GLFW_KEY_D) || Utilities::KeyPressed(PSP_CTRL_RIGHT) || Utilities::KeyHold(PSP_CTRL_RIGHT))) {
				run_state = 1;
			}
			else if (!(Utilities::KeyPressed(GLFW_KEY_A) || Utilities::KeyHold(GLFW_KEY_A) || Utilities::KeyPressed(PSP_CTRL_LEFT) || Utilities::KeyHold(PSP_CTRL_LEFT)) && (Utilities::KeyPressed(GLFW_KEY_D) || Utilities::KeyHold(GLFW_KEY_D) || Utilities::KeyPressed(PSP_CTRL_RIGHT) || Utilities::KeyHold(PSP_CTRL_RIGHT))) {
				run_state = 7;
			}
			else {
				run_state = 0;
			}
		}

		if ( !(Utilities::KeyPressed(GLFW_KEY_W) || Utilities::KeyHold(GLFW_KEY_W) || Utilities::KeyPressed(PSP_CTRL_UP) || Utilities::KeyHold(PSP_CTRL_UP)) && (Utilities::KeyPressed(GLFW_KEY_S) || Utilities::KeyHold(GLFW_KEY_S) || Utilities::KeyPressed(PSP_CTRL_DOWN) || Utilities::KeyHold(PSP_CTRL_DOWN))) {
			if ((Utilities::KeyPressed(GLFW_KEY_A) || Utilities::KeyHold(GLFW_KEY_A) || Utilities::KeyPressed(PSP_CTRL_LEFT) || Utilities::KeyHold(PSP_CTRL_LEFT)) && !(Utilities::KeyPressed(GLFW_KEY_D) || Utilities::KeyHold(GLFW_KEY_D) || Utilities::KeyPressed(PSP_CTRL_RIGHT) || Utilities::KeyHold(PSP_CTRL_RIGHT))) {
				run_state = 3;
			}
			else if (!(Utilities::KeyPressed(GLFW_KEY_A) || Utilities::KeyHold(GLFW_KEY_A) || Utilities::KeyPressed(PSP_CTRL_LEFT) || Utilities::KeyHold(PSP_CTRL_LEFT)) && (Utilities::KeyPressed(GLFW_KEY_D) || Utilities::KeyHold(GLFW_KEY_D) || Utilities::KeyPressed(PSP_CTRL_RIGHT) || Utilities::KeyHold(PSP_CTRL_RIGHT))) {
				run_state = 5;
			}
			else {
				run_state = 4;
			}
		}
		
		//So we aren't moving forward/backwards, just strafing
		if(run_state == -1){
			if ((Utilities::KeyPressed(GLFW_KEY_A) || Utilities::KeyHold(GLFW_KEY_A) || Utilities::KeyPressed(PSP_CTRL_LEFT) || Utilities::KeyHold(PSP_CTRL_LEFT)) && !(Utilities::KeyPressed(GLFW_KEY_D) || Utilities::KeyHold(GLFW_KEY_D) || Utilities::KeyPressed(PSP_CTRL_RIGHT) || Utilities::KeyHold(PSP_CTRL_RIGHT))) {
				run_state = 2;
			}
			else if (!(Utilities::KeyPressed(GLFW_KEY_A) || Utilities::KeyHold(GLFW_KEY_A) || Utilities::KeyPressed(PSP_CTRL_LEFT) || Utilities::KeyHold(PSP_CTRL_LEFT)) && (Utilities::KeyPressed(GLFW_KEY_D) || Utilities::KeyHold(GLFW_KEY_D) || Utilities::KeyPressed(PSP_CTRL_RIGHT) || Utilities::KeyHold(PSP_CTRL_RIGHT))) {
				run_state = 6;
			}
		}

		if(run_state != -1){
			positionChanged = true;
		}

		//XZ 
		glm::vec2 velocity = { 0, 0 };

		switch(run_state){
		case -1:
		default:
			break;

		case 0:
			velocity.x += glm::cos(glm::radians(yaw + 90));
			velocity.y += glm::sin(glm::radians(yaw + 90));
			break;

		case 4:
			velocity.x += -glm::cos(glm::radians(yaw + 90));
			velocity.y += -glm::sin(glm::radians(yaw + 90));
			break;

		case 1:
			velocity.x += glm::cos(glm::radians(yaw + 45));
			velocity.y += glm::sin(glm::radians(yaw + 45));
			break;

		case 7:
			velocity.x += glm::cos(glm::radians(yaw + 135));
			velocity.y += glm::sin(glm::radians(yaw + 135));
			break;

		case 3:
			velocity.x += -glm::cos(glm::radians(yaw + 135));
			velocity.y += -glm::sin(glm::radians(yaw + 135));
			break;

		case 5:
			velocity.x += -glm::cos(glm::radians(yaw + 45));
			velocity.y += -glm::sin(glm::radians(yaw + 45));
			break;

		case 2:
			velocity.x += glm::cos(glm::radians(yaw));
			velocity.y += glm::sin(glm::radians(yaw));
			break;

		case 6:
			velocity.x += -glm::cos(glm::radians(yaw));
			velocity.y += -glm::sin(glm::radians(yaw));
			break;
		}

		x += velocity.x * dt * 4.317;
		z += velocity.y * dt * 4.317;




		//Rotation
		glm::vec2 pos = Utilities::getCursorPos();
		
		if(pos != glm::vec2(-1,-1)){
#if CURRENT_PLATFORM != PLATFORM_PSP
			glm::vec2 change = pos - glm::vec2(Platform::PC::g_Window->getWidth() / 2, Platform::PC::g_Window->getHeight() / 2);
			yaw += change.x * 0.05f;
			pitch += change.y * 0.05f;

			if(change != glm::vec2(0, 0)){
				rotationChanged = true;
			}

			glfwSetCursorPos(Platform::PC::g_Window->getWindow(), Platform::PC::g_Window->getWidth() / 2, Platform::PC::g_Window->getHeight() / 2);
#endif
		}else{
			//Check console inputs
		}


		if (pitch > 89.99f)
			pitch = 89.99f;
		else if (pitch < -89.99f)
			pitch = -89.99f;

		if (yaw > 360)
			yaw -= 360;
		else if (yaw < 0)
			yaw += 360;


		//Send packet bois
		if(!positionChanged && !rotationChanged){
			Protocol::PacketsOut::send_player();
		}
		else if (positionChanged && !rotationChanged) {
			Protocol::PacketsOut::send_player_position();
		}
		else if (!positionChanged && rotationChanged) {
			Protocol::PacketsOut::send_player_look();
		}else{
			Protocol::PacketsOut::send_player_position_look();
		}

		positionChanged = false;
		rotationChanged = false;
	}

	void Player::draw()
	{
	}

}