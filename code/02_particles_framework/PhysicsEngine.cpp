#include "PhysicsEngine.h"
#include "Application.h"
#include "Camera.h"

using namespace glm;

const glm::vec3 GRAVITY = glm::vec3(0, -9.81, 0);
const float FRICTION = 0.9f;
const float BOUNCELOSS = -0.95f;


void ExplicitEuler(vec3& pos, vec3& vel, float mass, const vec3& accel, const vec3& impulse, float dt)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TODO: Implement
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

void SymplecticEuler(vec3& pos, vec3& vel, float mass, const vec3& accel, const vec3& impulse, float dt)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TODO: Implement
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

vec3 CollisionImpulse(Particle& pobj, const glm::vec3& cubeCentre, float cubeHalfExtent, float coefficientOfRestitution = 0.9f)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TODO: Calculate collision impulse
	vec3 impulse{ 0.0f };
	return impulse;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

vec3 BlowDryerForce(const vec3& particlePosition, float cone_y_base, float cone_y_tip, float cone_r_base, float max_force = 100)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TODO: Calculate blow dryer force
	vec3 force = {0,0,0};
	return force;
}

// This is called once
void PhysicsEngine::Init(Camera& camera, MeshDb& meshDb, ShaderDb& shaderDb)
{
	// Get a few meshes/shaders from the databases
	auto defaultShader = shaderDb.Get("default");
	auto particleMesh = meshDb.Get("tetra");
	auto groundMesh = meshDb.Get("plane");

	meshDb.Add("cube", Mesh(MeshDataFromWavefrontObj("resources/models/cube.obj")));
	meshDb.Add("sphere", Mesh(MeshDataFromWavefrontObj("resources/models/sphere.obj")));
	meshDb.Add("cone", Mesh(MeshDataFromWavefrontObj("resources/models/cone.obj")));
	auto mesh = meshDb.Get("cube");

	// Initialise ground
	ground.SetMesh(groundMesh);
	ground.SetShader(defaultShader);
	ground.SetScale(vec3(10.0f));

	boundingBox.SetMesh(mesh);
	boundingBox.SetShader(defaultShader);
	boundingBox.SetScale(vec3(5.0f));
	boundingBox.SetPosition(vec3(0, 5, 0));

	// Initialise particle
	particle.SetMesh(mesh);
	particle.SetShader(defaultShader);
	particle.SetColor(vec4(1, 0, 0, 1));
	particle.SetPosition(vec3(0, 5, 0));
	particle.SetScale(vec3(0.1f));
	particle.SetVelocity(vec3(1.f, 0.0f, 2.f));

	camera = Camera(vec3(0, 2.5, 10));
}

// This is called every frame
void PhysicsEngine::Update(float deltaTime, float totalTime)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TODO: Handle collisions and calculate impulse
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto impulse = CollisionImpulse(particle, glm::vec3(0.0f,5.0f,0.0f), 5.0f, 1.0f);
	// Calculate acceleration by accumulating all forces (here we just have gravity) and dividing by the mass
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TODO: Implement a simple integration scheme
	//vec3 p = particle.Position(), v = particle.Velocity();
	//vec3 acceleration = vec3(0.0f);
	//SymplecticEuler(p,v, particle.Mass(), acceleration, impulse, deltaTime);
	//particle.SetPosition(p);
	//particle.SetVelocity(v);

	vec3 windF = vec3(0, 0, 0);

	//Drag
	vec3 dragF = ((particle.Velocity() * particle.Velocity()) / vec3(2)) * vec3(0.25);
	if (particle.Velocity().x > 0)
		dragF *= vec3(-1, 1, 1);
	if (particle.Velocity().y > 0)
		dragF *= vec3(1, -1, 1);
	if (particle.Velocity().z > 0)
		dragF *= vec3(1, 1, -1);

	vec3 force = windF + dragF;
	vec3 acceleration = (force / particle.Mass()) + GRAVITY;

	particle.SetVelocity(particle.Velocity() + (deltaTime * acceleration));
	particle.SetPosition(particle.Position() + deltaTime * particle.Velocity());

	//Collision with boundaries
	if (particle.Position().x < boundingBox.Position().x - 5)
	{
		particle.SetPosition(vec3(boundingBox.Position().x - 5, particle.Position().y, particle.Position().z));
		particle.SetVelocity(particle.Velocity() * vec3(BOUNCELOSS, FRICTION, FRICTION));
	}
	if (particle.Position().x > boundingBox.Position().x + 5)
	{
		particle.SetPosition(vec3(boundingBox.Position().x + 5, particle.Position().y, particle.Position().z));
		particle.SetVelocity(particle.Velocity() * vec3(BOUNCELOSS, FRICTION, FRICTION));
	}

	if (particle.Position().y < boundingBox.Position().y - 5)
	{
		particle.SetPosition(vec3(particle.Position().x, boundingBox.Position().y - 5, particle.Position().z));
		particle.SetVelocity(particle.Velocity() * vec3(FRICTION, BOUNCELOSS, FRICTION));
	}
	if (particle.Position().y > boundingBox.Position().y + 5)
	{
		particle.SetPosition(vec3(particle.Position().x, boundingBox.Position().y + 5, particle.Position().z));
		particle.SetVelocity(particle.Velocity() * vec3(FRICTION, BOUNCELOSS, FRICTION));
	}

	if (particle.Position().z < boundingBox.Position().z - 5)
	{
		particle.SetPosition(vec3(particle.Position().x, particle.Position().y, boundingBox.Position().z - 5));
		particle.SetVelocity(particle.Velocity() * vec3(FRICTION, FRICTION, BOUNCELOSS));
	}
	if (particle.Position().z > boundingBox.Position().z + 5)
	{
		particle.SetPosition(vec3(particle.Position().x, particle.Position().y, boundingBox.Position().z + 5));
		particle.SetVelocity(particle.Velocity() * vec3(FRICTION, FRICTION, BOUNCELOSS));
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

// This is called every frame, after Update
void PhysicsEngine::Display(const mat4& viewMatrix, const mat4& projMatrix)
{
	particle.Draw(viewMatrix, projMatrix);
	ground.Draw(viewMatrix, projMatrix);
}

void PhysicsEngine::HandleInputKey(int keyCode, bool pressed)
{
	switch (keyCode)
	{
	case GLFW_KEY_1:
		printf("Key 1 was %s\n", pressed ? "pressed" : "released");
		break; // don't forget this at the end of every "case" statement!
	default:
		break;
	}
}