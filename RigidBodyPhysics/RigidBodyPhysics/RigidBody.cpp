#include "RigidBody.h"

RigidBody::RigidBody()
{
	currentState = State(1.0f,1.0f);
	previousState = currentState;
	//springPos = glm::vec3(0.0f, 5.0f, 0.0f);
	hasSpring = false;
	k=3.0f;
	b=0.2f;
	userData = nullptr;
}

RigidBody::RigidBody(float length, float mass, int id)
{
	ID = id;
	boundingBox = AABB(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	collider = ConvexMesh(0, length);
	currentState = State(length, mass);
	previousState = currentState;
	generateVertices(length);
	//springPos = glm::vec3(0.0f,5.0f,0.0f);
	hasSpring = false;
	k=3.0f;
	b=0.2f;
	userData = nullptr;
}

RigidBody::RigidBody(float mass, int id, vector<glm::vec3> &vPositions, vector<glm::vec4> &vColors, vector<glm::vec3> &vNormals, vector<GLuint> &vIndices)
{
	ID = id;
	collider = ConvexMesh(vPositions, vColors, vNormals, vIndices);
	boundingBox = AABB(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	for(int i=0; i<vPositions.size(); i++)
	{
		vertexPositions.push_back(vPositions[i]);
		originalVertexPositions.push_back(vPositions[i]);
	}
	calcBoundingBox();
	
	//Inertia Tensor Calculation
	float w = (boundingBox.maxX - boundingBox.minX);
	float h = (boundingBox.maxY - boundingBox.minX);
	float d = (boundingBox.maxZ - boundingBox.minZ);
	
	float Ix = ((1/12)*mass*((h*h) + (d*d)));
	float Iy = ((1/12)*mass*((w*w) + (d*d)));
	float Iz = ((1/12)*mass*((w*w) + (h*h)));
	
	glm::mat3 iTensor = glm::mat3(	Ix, 0.0f, 0.0f,
									0.0f, Iy, 0.0f,
									0.0f, 0.0f, Iz);
	currentState = State(iTensor, mass);
	previousState = currentState;

	hasSpring = false;
	k=3.0f;
	b=0.2f;
	userData = nullptr;
}


void RigidBody::Update(float t, float dt)
{
	previousState = currentState;
	Integrate(currentState, t, dt);
}

void RigidBody::Draw()
{
	collider.Draw();
}

Derivative RigidBody::Evaluate(const State &initial, float t, float dt, const Derivative &d)
{
	State state;
	//Euler Integration Step
	state.position = initial.position + (d.velocity*dt);
	state.linearMomentum = initial.linearMomentum + (d.force*dt);
	state.orientation = initial.orientation + (d.spin*dt);
	state.angularMomentum = initial.angularMomentum + (d.torque*dt);

	//Calculate Secondary Variables and change vertex positons
	state.recalculate();
	applyTransform(state);

	//Apply forces and generate result
	Derivative output;
	output.velocity = state.linearVelocity;
	output.spin = state.spin;
	Force(state, output, t+dt);
	Torque(state, output, t+dt);

	return output;
}

void RigidBody::Force(const State &state, Derivative &derivative, float t)
{
	//Calculate Spring Force
	if(hasSpring)
	{
		glm::vec3 x = vertexPositions[0] - springPos;
		glm::vec3 v = state.linearVelocity + glm::cross(state.angularVelocity, (vertexPositions[0] - state.position));

		derivative.force = -k*x - b*v;
		derivative.force += glm::vec3(0.0f, -9.81f, 0.0f);
		derivative.force -= 0.5f*state.linearVelocity;
	}
	else
	{
		derivative.force = glm::vec3(0.0f, -9.81f, 0.0f);
		derivative.force -= 0.9f*state.linearVelocity;
	}
}

void RigidBody::Torque(const State &state, Derivative &derivative, float t){
	if(hasSpring)
	{
		//Calculate Torque resulting from applied force;
		glm::vec3 x = vertexPositions[0] - springPos;
		glm::vec3 v = state.linearVelocity + glm::cross(state.angularVelocity, (vertexPositions[0] - state.position));


		glm::vec3 datForce = -k*x - b*v;

		derivative.torque = glm::cross(vertexPositions[0] - state.position, datForce);
	}
	derivative.torque = glm::cross(vertexPositions[0] - state.position, derivative.force);
}

void RigidBody::Integrate(State &state, float t, float dt)
{
	//RK4 Integration
	
	//Get Derivatives
	Derivative a = Evaluate(state, t, 0.0f, Derivative());
	Derivative b = Evaluate(state, t, 0.5f*dt, a);
	Derivative c = Evaluate(state, t, 0.5f*dt, b);
	Derivative d = Evaluate(state, t, dt, c);

	//Calculate 3D movement paramters
	const glm::vec3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
	const glm::vec3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
	const glm::quat drdt = 1.0f/6.0f * (a.spin + 2.0f*(b.spin + c.spin) + d.spin);
	const glm::vec3 dldt = 1.0f/6.0f * (a.torque + 2.0f*(b.torque + c.torque) + d.torque);

	//Integrate
	state.position = state.position + dxdt * dt;
	state.linearMomentum = state.linearMomentum + dpdt * dt;
	state.orientation = state.orientation + drdt * dt;
	state.angularMomentum = state.angularMomentum + dldt * dt;

	state.recalculate();
	applyTransform(state);
}

void RigidBody::generateVertices(float length){
	float dist = length/2.0f;

	//FrontFace
	vertexPositions.push_back(glm::vec3(-dist, dist, dist));		//0
	vertexPositions.push_back(glm::vec3(dist, dist, dist));			//1
	vertexPositions.push_back(glm::vec3(dist, -dist, dist));		//2
	vertexPositions.push_back(glm::vec3(-dist, -dist, dist));		//3

	//RightFace
	//vertexPositions.push_back(glm::vec3(dist, dist, dist));			//4
	//vertexPositions.push_back(glm::vec3(dist, dist, -dist));		//5
	//vertexPositions.push_back(glm::vec3(dist, -dist, -dist));		//6
	//vertexPositions.push_back(glm::vec3(dist, -dist, dist));		//7

	//BackFace
	vertexPositions.push_back(glm::vec3(dist, dist, -dist));		//8
	vertexPositions.push_back(glm::vec3(-dist, dist, -dist));		//9
	vertexPositions.push_back(glm::vec3(-dist, -dist, -dist));		//10
	vertexPositions.push_back(glm::vec3(dist, -dist, -dist));		//11

	//LeftFace
	//vertexPositions.push_back(glm::vec3(-dist, dist, -dist));		//12
	//vertexPositions.push_back(glm::vec3(-dist, dist, dist));		//13
	//vertexPositions.push_back(glm::vec3(-dist, -dist, dist));		//14
	//vertexPositions.push_back(glm::vec3(-dist, -dist, -dist));		//15

	//TopFace
	//vertexPositions.push_back(glm::vec3(-dist, dist, -dist));		//16
	//vertexPositions.push_back(glm::vec3(dist, dist, -dist));		//17
	//vertexPositions.push_back(glm::vec3(dist, dist, dist));			//18
	//vertexPositions.push_back(glm::vec3(-dist, dist, dist));		//19

	//BottomFace
	//vertexPositions.push_back(glm::vec3(-dist, -dist, dist));		//20
	//vertexPositions.push_back(glm::vec3(dist, -dist, dist));		//21
	//vertexPositions.push_back(glm::vec3(dist, -dist, -dist));		//22
	//vertexPositions.push_back(glm::vec3(-dist, -dist, -dist));		//23

	//Other Vector
	//FrontFace
	originalVertexPositions.push_back(glm::vec3(-dist, dist, dist));		//0
	originalVertexPositions.push_back(glm::vec3(dist, dist, dist));			//1
	originalVertexPositions.push_back(glm::vec3(dist, -dist, dist));		//2
	originalVertexPositions.push_back(glm::vec3(-dist, -dist, dist));		//3

	//RightFace
	//originalVertexPositions.push_back(glm::vec3(dist, dist, dist));			//4
	//originalVertexPositions.push_back(glm::vec3(dist, dist, -dist));		//5
	//originalVertexPositions.push_back(glm::vec3(dist, -dist, -dist));		//6
	//originalVertexPositions.push_back(glm::vec3(dist, -dist, dist));		//7

	//BackFace
	originalVertexPositions.push_back(glm::vec3(dist, dist, -dist));		//8
	originalVertexPositions.push_back(glm::vec3(-dist, dist, -dist));		//9
	originalVertexPositions.push_back(glm::vec3(-dist, -dist, -dist));		//10
	originalVertexPositions.push_back(glm::vec3(dist, -dist, -dist));		//11

	//LeftFace
	//originalVertexPositions.push_back(glm::vec3(-dist, dist, -dist));		//12
	//originalVertexPositions.push_back(glm::vec3(-dist, dist, dist));		//13
	//originalVertexPositions.push_back(glm::vec3(-dist, -dist, dist));		//14
	//originalVertexPositions.push_back(glm::vec3(-dist, -dist, -dist));		//15

	//TopFace
	//originalVertexPositions.push_back(glm::vec3(-dist, dist, -dist));		//16
	//originalVertexPositions.push_back(glm::vec3(dist, dist, -dist));		//17
	//originalVertexPositions.push_back(glm::vec3(dist, dist, dist));			//18
	//originalVertexPositions.push_back(glm::vec3(-dist, dist, dist));		//19

	//BottomFace
	//originalVertexPositions.push_back(glm::vec3(-dist, -dist, dist));		//20
	//originalVertexPositions.push_back(glm::vec3(dist, -dist, dist));		//21
	//originalVertexPositions.push_back(glm::vec3(dist, -dist, -dist));		//22
	//originalVertexPositions.push_back(glm::vec3(-dist, -dist, -dist));		//23
}

void RigidBody::applyTransform(State state){
	int size = vertexPositions.size();

	for(int i = 0; i<size; i++){
		vertexPositions[i] = state.modelMatrix * originalVertexPositions[i];
	}
}

void RigidBody::calcBoundingBox(){
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;
	float maxZ = -FLT_MAX;
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;

	for(int i=0; i<vertexPositions.size(); i++)
	{
		if(vertexPositions[i].x < minX){
			minX = vertexPositions[i].x;
		}
		else if(vertexPositions[i].x > maxX){
			maxX = vertexPositions[i].x;
		}

		if(vertexPositions[i].y < minY){
			minY = vertexPositions[i].y;
		}
		else if(vertexPositions[i].y > maxY){
			maxY = vertexPositions[i].y;
		}
		
		if(vertexPositions[i].z < minZ){
			minZ = vertexPositions[i].z;
		}
		else if(vertexPositions[i].z > maxZ){
			maxZ = vertexPositions[i].z;
		}

	}

	boundingBox.Update(maxX, maxY, maxZ, minX, minY, minZ);
}

void RigidBody::setBroadColliding(bool state){
	isBroadColliding = state;
	boundingBox.setBroadColliding(state);
	//boundingBox.isBroadColliding = state;
}

void RigidBody::setNarrowColliding(bool state){
	isNarrowColliding = state;
	//boundingBox.isColliding = true;
}

void RigidBody::translateGeometry(glm::vec3 trans)
{
	//Things go here
}

void RigidBody::rotateGeometry(glm::quat rot)
{
	//Things go here
}

void RigidBody::attachSpring(glm::vec3 sPos)
{
	springPos = sPos;
	hasSpring = true;
}

void RigidBody::detachSpring()
{
	hasSpring = false;
}

AABB *RigidBody::GetBoundingBox()
{
	return &boundingBox;
}