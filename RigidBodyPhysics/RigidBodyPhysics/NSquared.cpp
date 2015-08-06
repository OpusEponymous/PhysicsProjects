#include "NSquared.h"

NSquared::NSquared()
{
	//Do Nothing
}

void NSquared::Add(RigidBody *rBody)
{
	ColliderList.push_back(rBody);
}

void NSquared::Update()
{
	//Do feck all
}

vector<pair<RigidBody *, RigidBody *>> &NSquared::ComputePairs()
{
	ColliderPairList.clear();

	//Outer Loop
	for(vector<RigidBody *>::iterator i = ColliderList.begin(); i != ColliderList.end(); ++i)
	{
		vector<RigidBody *>::iterator jStart = i;
		for(vector<RigidBody *>::iterator j = ++jStart; j != ColliderList.end(); ++j)
		{
			RigidBody *rBodyA = *i;
			RigidBody *rBodyB = *j;
			AABB *aabbA = rBodyA->GetBoundingBox();
			AABB *aabbB = rBodyB->GetBoundingBox();

			if(aabbA->checkCollision(*aabbB))
			{
				ColliderPairList.push_back(make_pair(rBodyA, rBodyB));
			}
			
		}
	}

	return ColliderPairList;
}

RigidBody *NSquared::Pick(const glm::vec3 &point)
{
	for(vector<RigidBody *>::iterator i = ColliderList.begin(); i != ColliderList.end(); ++i)
	{
		RigidBody *rBody = *i;
		if(rBody->boundingBox.Contains(point))
		{
			return rBody;
		}
	}
	return nullptr;
}

void NSquared::Query(AABB &aabb, vector<RigidBody *> &output)
{
	for(vector<RigidBody *>::iterator i = ColliderList.begin(); i != ColliderList.end(); ++i)
	{
		RigidBody *rBody = *i;
		if (rBody->boundingBox.checkCollision(aabb))
		{
			output.push_back(rBody);
		}
	}
}

RayCastResult NSquared::RayCast(const Ray3 &ray)
{
	//Brute Force

	//Populate Candidate List
	vector<RigidBody *> candidateList;
	candidateList.reserve(ColliderList.size());
	for(vector<RigidBody *>::iterator i = ColliderList.begin(); i != ColliderList.end(); ++i)
	{
		RigidBody *rBody = *i;
		if(rBody->boundingBox.TestRay(ray))
		{
			candidateList.push_back(rBody);
		}
	}

	struct ResultEntry
	{
		RigidBody *rBody;
		float t;
		glm::vec3 normal;

		bool operator<(const ResultEntry &rhs) const
		{
			return t >rhs.t;
		}
	};

	vector<ResultEntry> resultList;
	resultList.reserve(candidateList.size());
	for(vector<RigidBody *>::iterator i = candidateList.begin(); i != candidateList.end(); ++i)
	{
		RigidBody *rBody = *i;
		float t;
		glm::vec3 normal;

		//Fucking Collider Test Ray what is this doing
	}

	//Just to get everything to compile come back to this later
	RayCastResult dummy;
	return dummy;

}