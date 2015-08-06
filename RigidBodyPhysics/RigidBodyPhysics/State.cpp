#include "State.h"

State::State()
{
	size = 1.0f;										//Cube Specific
	mass = 1.0f;
	inverseMass = 1.0f / mass;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	linearMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
	orientation = glm::quat();
	angularMomentum = glm::vec3(0.0f,0.0f,0.0f);
	float inertia = mass * size * size * 1.0f/6.0f;		//Cube Specific
	inertiaTensor = glm::mat3(inertia);					//Cube Specific
	inverseInertiaTensor = glm::mat3(1.0f/inertia);		//Cube Specific
	recalculate();
}

State::State(float l, float m)
{
	size = l;											//Cube Specific
	mass = m;	
	inverseMass = 1.0f / mass;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	linearMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
	orientation = glm::quat();
	angularMomentum = glm::vec3(0.0f,0.0f,0.0f);
	float inertia = mass * size * size * 1.0f/6.0f;		//Cube Specific
	inertiaTensor = glm::mat3(inertia);					//Cube Specific
	inverseInertiaTensor = glm::mat3(1.0f/inertia);		//Cube Specific
	recalculate();
}

State::State(glm::mat3 iTensor, float m)
{
	mass = m;
	inverseMass = 1.0f / mass;
	position = glm::vec3(0.0f,0.0f,0.0f);
	linearMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
	orientation = glm::quat();
	angularMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
	inertiaTensor = iTensor;
	inverseInertiaTensor = glm::inverse(iTensor);
	recalculate();
}

void State::recalculate()
{
	linearVelocity = linearMomentum * inverseMass;

	angularVelocity = angularMomentum * inverseInertiaTensor;

	orientation = glm::normalize(orientation);

	spin = 0.5f * glm::quat(0, angularVelocity.x, angularVelocity.y, angularVelocity.z) * orientation;

	glm::mat4 translation = glm::translate(position);

	modelMatrix = translation * glm::mat4_cast(orientation);

	invModelMatrix = modelMatrix._inverse();
}

