#include "Camera.h"

//const static float STEP_SCALE = 0.1f;
const static float STEP_SCALE = 1.0f;
const static int MARGIN = 100;

Camera::Camera()
{
	vMat = glm::mat4(1.0f);
}

Camera::Camera(float r, float theta, float phi, glm::vec3 cTarget, glm::vec3 cUP)
{
	//vMat = glm::lookAt(cPos, cTarget, cUP);
	R = r;
	Theta = theta;
	Phi = phi;
	target = cTarget;
	up = cUP;
	isOrbital = true;
	calcMatrix();
}

Camera::Camera(int WindowWidth, int WindowHeight, glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up)
{
	windowWidth = WindowWidth;
	windowHeight = WindowHeight;

	target = Target;
	up = Up;
	position = Pos;

	isOrbital = false;

	Init();
	
}

void Camera::calcMatrix(){

	if(isOrbital)
	{
		float x = R * glm::sin(glm::radians(Theta)) * glm::sin(glm::radians(Phi));
		float y = R * glm::cos(glm::radians(Theta));
		float z = R * glm::sin(glm::radians(Theta)) * glm::cos(glm::radians(Phi));
	
		position = glm::vec3(x,y,z);

		vMat = glm::lookAt(position, target, up);
	}
	else
	{
		//vMat = glm::lookAt(position, target, up);
		vMat = glm::lookAt(position, position - target, up);
	}

}

void Camera::calcMatrix(glm::vec3 targetPt)
{
	float x = R * glm::sin(glm::radians(Theta)) * glm::sin(glm::radians(Phi));
	float y = R * glm::cos(glm::radians(Theta));
	float z = R * glm::sin(glm::radians(Theta)) * glm::cos(glm::radians(Phi));

	position = glm::vec3(x,y,z);

	vMat = glm::lookAt(position + targetPt, targetPt, up);

}

bool Camera:: handleKeyboardOrbital(int Key)
{
	bool ret = false;
	float inc = 2.0f;

	switch(Key){
		case GLUT_KEY_UP:
		{
			Phi = Phi + inc;
		}
		break;

		case GLUT_KEY_DOWN:
		{
			Phi = Phi - inc;
		}
		break;

		case GLUT_KEY_LEFT:
		{
			Theta = Theta + inc;
			
		}
		break;

		case GLUT_KEY_RIGHT:
		{
			Theta = Theta - inc;
			if (Theta <= 0.0f)
			{
				Theta = 1.0f;
			}
		}
		break;

		case GLUT_KEY_PAGE_UP:
		{
			R = R - 0.2f;
		}
		break;

		case GLUT_KEY_PAGE_DOWN:
		{
			R = R + 0.2f;
		}
		break;
	}

	return true;
}

bool Camera::handleKeyboardFPS(int key)
{
	bool ret = false;

	switch (key)
	{
		case GLUT_KEY_UP:
		{
			position -= (target * STEP_SCALE);
			ret = true;
		}
		break;
		case GLUT_KEY_DOWN:
		{
			position += (target * STEP_SCALE);
			ret = true;
		}
		break;
		case GLUT_KEY_LEFT:
		{
			glm::vec3 left = glm::cross(target,up);
			left = glm::normalize(left);
			left *= STEP_SCALE;
			position += left;
			ret = true;
		}
		break;
		case GLUT_KEY_RIGHT:
		{
			glm::vec3 right = glm::cross(up,target);
			right = glm::normalize(right);
			right *= STEP_SCALE;
			position += right;
			ret = true;
		}
		break;
	}

	return ret;
}

void Camera::Init()
{
	glm::vec3 hTarget(target.x, 0.0f, target.y);
	hTarget	= glm::normalize(hTarget);

	if(hTarget.z >= 0.0f)
	{
		if(hTarget.x >= 0.0f)
		{
			Theta = 360.0f - glm::degrees(glm::asin(hTarget.z));
		}
		else
		{
			Theta = 180.0f + glm::degrees(glm::asin(hTarget.z));
		}
	}
	else
	{
		if(hTarget.x >= 0.0f)
		{
			Theta = glm::degrees(glm::asin(-hTarget.z));
		}
		else
		{
			Theta = 90.0f + glm::degrees(glm::asin(-hTarget.z));
		}
	}

	Phi = - glm::degrees(glm::asin(target.y));

	onUpperEdge = false;
	onLowerEdge = false;
	onLeftEdge = false;
	onRightEdge = false;

	mousePos.x = windowWidth / 2;
	mousePos.y = windowHeight / 2;

	glutWarpPointer(mousePos.x, mousePos.y);
}

void Camera::Update()
{
	glm::vec3 vAxis(0.0f, 1.0f, 0.0f);

	//Rotate the view vector by the horizontal angle around the vertical axis
	glm::vec3 vView(1.0f, 0.0f, 0.0f);
	vView = glm::rotate(vView, Theta, vAxis);
	vView = glm::normalize(vView);

	//Rotate the view vector by the vertical angle around the horizontal axis
	glm::vec3 hAxis = glm::cross(vAxis, vView);
	hAxis = glm::normalize(hAxis);
	vView = glm::rotate(vView, Phi, hAxis);

	target = vView;
	target = glm::normalize(target);

	up = glm::cross(target, hAxis);
	up = glm::normalize(up);

}

void Camera::handleMouse(int x, int y)
{
	int DeltaX = x - mousePos.x;
	int DeltaY = y - mousePos.y;

	mousePos.x = x;
	mousePos.y = y;

	//Theta += (float)DeltaX / 20.0f;
	//Phi += (float)DeltaY / 20.0f;

	Theta -= (float)DeltaX / 20.0f;
	Phi -= (float)DeltaY / 20.0f;

	//Check Case: No Movement but at screen's edge
	if(DeltaX == 0)
	{
		if(x <= MARGIN)
		{
			onLeftEdge = true;
		}
		else if (x >= (windowWidth - MARGIN))
		{
			onRightEdge = true;
		}
	}
	else
	{
		onLeftEdge = false;
		onRightEdge = false;
	}

	if(DeltaY = 0)
	{
		if(y <= MARGIN)
		{
			onUpperEdge = true;
		}
		else if(y >= (windowHeight - MARGIN))
		{
			onLowerEdge = true;
		}
	}
	else
	{
		onUpperEdge = false;
		onLowerEdge = false;
	}

	Update();
}

void Camera::onRender()
{
	bool update = false;

	if(onLeftEdge)
	{
		Theta += 1.5f;
		//Theta -= 1.5f;
		update = true;
	}
	else if(onRightEdge)
	{
		Theta -= 1.5f;
		//Theta += 1.5f;
		update = true;
	}
	if(onUpperEdge)
	{
		if(Phi > -90.0f)
		{
			Phi += 1.5f;
			//Phi -= 1.5f;
			update = true;
		}
	}
	else if(onLowerEdge)
	{
		if(Phi < 90.0f)
		{
			Phi -= 1.5f;
			//Phi += 1.5f;
			update = true;
		}
	}

	if(update)
	{
		Update();
	}
}