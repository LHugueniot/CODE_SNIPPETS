#include "ecs.h"

struct float3{
	float3(float _x, float _y, float _z) : 
		x(_x)
		y(_y)
		z(_z)
	{}
	float x;
	float y; 
	float z;
};

struct Gravity{
	float3 force; 
};

struct RigidBody{
	float3 velocity;
	float3 acceleration;
};

struct Transform{
	float3 position;
	float3 rotation;
	float3 scale;
};

struct PhysicsSystem{

	void Update(float dt){
		for (auto const& entity : m_entities){
			auto& rigidBody = g_coordinator.GetComponent<RigidBody>(entity);
			auto& transform = g_coordinator.GetComponent<Transform>(entity);
			auto const& gravity = g_coordinator.GetComponent<Gravity>(entity);
	
			transform.position += rigidBody.velocity * dt;
	
			rigidBody.velocity += gravity.force * dt;
		}
	}
	std::vector<Entity> m_entities;
}

Coordinator g_coordinator;

int main(int argc, char const *argv[]){
    for(auto const & entity : m_entities){

		auto& rigidBody = GetComponent<RigidBody>(entity);
		auto& transform = GetComponent<Transform>(entity);
		auto const& gravity = GetComponent<Gravity>(entity);

		transform.position += rigidBody.velocity * dt;

		rigidBody.velocity += gravity.force * dt;
    }
	/* code */
	return 0;
}