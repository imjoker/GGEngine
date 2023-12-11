// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Assets/Camera.h>
#include <Engine/Math/cMatrix_transformation.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{

		Logging::OutputMessage("\nMyGame - Escape pressed - Exiting the application ----------- FuncName - %s, File - %s, Line - %d\n", __func__, __FILE__, __LINE__);

		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	PressedKeyCode = UserInput::KeyCodes::None;

	// Is the user pressing the key?
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		PressedKeyCode = UserInput::KeyCodes::Space;
		activeGameObjects[0]->SetMesh(MeshEffectPairs[1].mesh);
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Control))
	{
		PressedKeyCode = UserInput::KeyCodes::Control;
	} else
		activeGameObjects[0]->SetMesh(MeshEffectPairs[0].mesh);

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
		camera.AddVelocity(Math::sVector(0.0f, 0.0f, -playerVelocity));

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
		camera.AddVelocity(Math::sVector(0.0f, 0.0f, playerVelocity));

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
		camera.AddVelocity(Math::sVector(-playerVelocity, 0.0f, 0.0f));

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
		camera.AddVelocity(Math::sVector(playerVelocity, 0.0f, 0.0f));

	if (UserInput::IsKeyPressed('W'))
		activeGameObjects[0]->AddVelocity(Math::sVector(0.0f, playerVelocity, 0.0f));

	if (UserInput::IsKeyPressed('A'))
		activeGameObjects[0]->AddVelocity(Math::sVector(-playerVelocity, 0.0f, 0.0f));

	if (UserInput::IsKeyPressed('S'))
		activeGameObjects[0]->AddVelocity(Math::sVector(0.0f, -playerVelocity, 0.0f));

	if (UserInput::IsKeyPressed('D'))
		activeGameObjects[0]->AddVelocity(Math::sVector(playerVelocity, 0.0f, 0.0f));		
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	for (Assets::GameObject* currObj : activeGameObjects)
	{
		currObj->Update(i_elapsedSecondCount_sinceLastUpdate);
	}
	
	camera.Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	eae6320::cResult result;
	
	Logging::Initialize();

	Logging::OutputMessage("\nMyGame - Initialize Successful ----------- FuncName - %s, File - %s, Line - %d\n", __func__, __FILE__, __LINE__);

	// Initialize the shading data
	{
		if (!(result = eae6320::Graphics::cEffect::Load("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/sample.shader", MeshEffectPairs[0].effect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
		if (!(result = eae6320::Graphics::cEffect::Load("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/sample2.shader", MeshEffectPairs[1].effect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
		if (!(result = eae6320::Graphics::cEffect::Load("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/sample2.shader", MeshEffectPairs[2].effect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}

	// Initialize the geometry
	{
		// 1st mesh
		MeshEffectPairs[0].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/ring.mesh");

		if (!MeshEffectPairs[0].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}
	{
		// 2nd mesh
		MeshEffectPairs[1].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/cone.mesh");

		if (!MeshEffectPairs[1].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}
	{
		// 3rd mesh
		MeshEffectPairs[2].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/ball.mesh");

		if (!MeshEffectPairs[2].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}

	activeGameObjects.reserve(10);

	player = new eae6320::Assets::GameObject(MeshEffectPairs[0].mesh, MeshEffectPairs[0].effect, Physics::sRigidBodyState());
	npc = new eae6320::Assets::GameObject(MeshEffectPairs[2].mesh, MeshEffectPairs[2].effect, Physics::sRigidBodyState());

	activeGameObjects.push_back(player);
	activeGameObjects.push_back(npc);

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	while (!activeGameObjects.empty())
	{
		Assets::GameObject* currObj = activeGameObjects.back();
		activeGameObjects.pop_back();
		delete currObj;
	}

	for (unsigned int i = 0; i < numMeshEffectPairs; ++i)
	{
		MeshEffectPairs[i].mesh->DecrementReferenceCount();
		MeshEffectPairs[i].mesh = nullptr;

		MeshEffectPairs[i].effect->DecrementReferenceCount();
		MeshEffectPairs[i].effect = nullptr;
	}

	Logging::CleanUp();

	Logging::OutputMessage("\nMyGame - CleanUp Successful ----------- FuncName - %s, File - %s, Line - %d\n", __func__, __FILE__, __LINE__);

	return Results::Success;
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Graphics::ObjectRenderData* renderableObjects = new Graphics::ObjectRenderData[activeGameObjects.size()];

	unsigned int i = 0;

	for (Assets::GameObject * currObj : activeGameObjects)
	{
		renderableObjects[i].mesh = currObj->GetMesh();
		renderableObjects[i].effect = currObj->GetEffect();
		renderableObjects[i].drawData.g_transform_localToWorld = currObj->GetTransformLocalToWorld(i_elapsedSecondCount_sinceLastSimulationUpdate);

		++i;
	}

	switch (PressedKeyCode) {

		case UserInput::KeyCodes::Space:

		default:
			Graphics::SetBackgroundColor(Graphics::Green);
			break;
	}

	Graphics::SetRenderableObjects(renderableObjects, (uint16_t) activeGameObjects.size());

	Graphics::SetCameraTransforms(camera.CreateWorldToCameraTransform(i_elapsedSecondCount_sinceLastSimulationUpdate), camera.CreateCameraToProjectedTransform_perspective());
}
