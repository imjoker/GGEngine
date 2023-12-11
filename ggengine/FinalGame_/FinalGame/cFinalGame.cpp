// Includes
//=========

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Assets/Camera.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/CZPhysics/CZPhysics.h>
#include <Engine/CZPhysics/BoxCollider.h>
#include <Engine/Math/Functions.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cFinalGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{

		Logging::OutputMessage("\nFinalGame - Escape pressed - Exiting the application ----------- FuncName - %s, File - %s, Line - %d\n", __func__, __FILE__, __LINE__);

		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cFinalGame::UpdateSimulationBasedOnInput()
{
	if (UserInput::IsKeyPressed('W'))
		playerVelocity = Math::sVector(0.0f, 0.25f, 0.0f);

	if (UserInput::IsKeyPressed('A'))
		playerVelocity = Math::sVector(-0.25f, 0.0f, 0.0f);

	if (UserInput::IsKeyPressed('S'))
		playerVelocity = Math::sVector(0.0f, -0.25f, 0.0f);

	if (UserInput::IsKeyPressed('D'))
		playerVelocity = Math::sVector(0.25f, 0.0f, 0.0f);
}

void eae6320::cFinalGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	if (((eae6320::Player*)player)->isDead) {

		//exit(0);
		// Exit the application
		const auto result = Exit(EXIT_SUCCESS);
		EAE6320_ASSERT(result);
	}
		
	// Assuming you have a constant rotation speed in degrees per second
	const float rotationSpeed = 2.0f; // 90 degrees per second, adjust as needed

	// Create a unit vector representing the rotation axis
	Math::sVector rotationAxis = Math::sVector(0.0f, 0.0f, 1.0f); // Y-axis rotation, adjust as needed
	rotationAxis.Normalize();

	// Create the angular velocity vector
	Math::sVector angularVelocity = rotationSpeed * rotationAxis;

	rotator->GetRigidBody()->SetAngularVelocity(angularVelocity);
	
	player->AddVelocity(playerVelocity);

	if (!npc->HasReachedTarget(player->GetPosition())) {

		if ((path.size() == 0) && npc->HasReachedTarget(currentNPCTargetLoc))
			path = PathFinding::GetPath(PathFinding::Grid::GetGrid()->GetNodeAt((int)npc->GetPosition().x, (int)npc->GetPosition().y, (int)npc->GetPosition().z), PathFinding::Grid::GetGrid()->GetNodeAt((int)player->GetPosition().x, (int)player->GetPosition().y, (int)player->GetPosition().z));
		else {

			if (npc->HasReachedTarget(currentNPCTargetLoc)) {

				// if current target has been reached, get the next node to reach
				currentNPCTargetLoc = Math::sVector(path.back()->x, path.back()->y, path.back()->z);
				PathFinding::Node* node = path.back();
				path.pop_back();
				delete node;
			}

			npc->SetVelocityToReachTarget(currentNPCTargetLoc);
		}
	} else
		npc->SetVelocity(Math::sVector(0.0f, 0.0f, 0.0f));


	for (Assets::GameObject* currObj : activeGameObjects)
	{
		currObj->Update(i_elapsedSecondCount_sinceLastUpdate);
	}
	
	camera.Update(i_elapsedSecondCount_sinceLastUpdate);

	ChrisZ::Physics::Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::cResult result;
	
	Logging::Initialize();

	Logging::OutputMessage("\nFinalGame - Initialize Successful ----------- FuncName - %s, File - %s, Line - %d\n", __func__, __FILE__, __LINE__);

	// Initialize the shading data
	{
		if (!(result = eae6320::Graphics::cEffect::Load("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/sample.shader", MeshEffectPairs[0].effect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
		if (!(result = eae6320::Graphics::cEffect::Load("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/sample3.shader", MeshEffectPairs[1].effect)))
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
		MeshEffectPairs[0].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/ball.mesh");

		if (!MeshEffectPairs[0].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}
	{
		// 2nd mesh
		MeshEffectPairs[1].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/gear.mesh");

		if (!MeshEffectPairs[1].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}
	{
		// 3rd mesh
		MeshEffectPairs[2].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/horizontal.mesh");

		if (!MeshEffectPairs[2].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}
	{
		// 4th mesh
		MeshEffectPairs[3].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/vertical.mesh");

		if (!MeshEffectPairs[3].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}
	{
		// 5th mesh
		MeshEffectPairs[4].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/horizontalwall.mesh");

		if (!MeshEffectPairs[4].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}
	{
		// 6th mesh
		MeshEffectPairs[5].mesh = Graphics::VertexMesh::CreateMeshFromFile("data/meshes/verticalwall.mesh");

		if (!MeshEffectPairs[5].mesh)
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return eae6320::Results::Failure;
		}
	}

	// initialize a map
	PathFinding::Grid* grid = PathFinding::Grid::MakeGrid(150, 150, 150);

	activeGameObjects.reserve(10);

	player = new eae6320::Player(MeshEffectPairs[0].mesh, MeshEffectPairs[0].effect, Math::sVector(100.0f, 100.0f, 100.0f), Math::sVector(0.5f, 0.5f, 0.5f));

	npc = new eae6320::Assets::GameObject(MeshEffectPairs[1].mesh, MeshEffectPairs[2].effect, Math::sVector(110.0f, 110.0f, 100.0f), Math::sVector(0.5f, 0.5f, 0.5f));


	activeGameObjects.push_back(player);
	activeGameObjects.push_back(npc);

	// create map boundaries
	Assets::GameObject * leftBoundary	= new Assets::GameObject(MeshEffectPairs[3].mesh, MeshEffectPairs[2].effect, Math::sVector(131.0f, 100.0f, 100.0f), Math::sVector(0.5f, 35.0f, 0.5f));
	Assets::GameObject* rightBoundary	= new Assets::GameObject(MeshEffectPairs[3].mesh, MeshEffectPairs[2].effect, Math::sVector(69.0f, 100.0f, 100.0f), Math::sVector(0.5f, 35.0f, 0.5f));
	Assets::GameObject* topBoundary		= new Assets::GameObject(MeshEffectPairs[2].mesh, MeshEffectPairs[2].effect, Math::sVector(100.0f, 131.0f, 100.0f), Math::sVector(35.0f, 0.5f, 0.5f));
	Assets::GameObject* bottomBoundary	= new Assets::GameObject(MeshEffectPairs[2].mesh, MeshEffectPairs[2].effect, Math::sVector(100.0f, 69.0f, 100.0f), Math::sVector(35.0f, 0.5f, 0.5f));

	leftBoundary->GetCollider()->SetCenterOffset (Math::sVector(-1.0f, 0.0f, 0.0f));
	rightBoundary->GetCollider()->SetCenterOffset (Math::sVector(-1.0f, 0.0f, 0.0f));

	for (int j = 0; j < 36; ++j) {

		grid->GetNodeAt((int)leftBoundary->GetPosition().x, (int)leftBoundary->GetPosition().y + j, (int)leftBoundary->GetPosition().z)->isObstacle = true;
		grid->GetNodeAt((int)rightBoundary->GetPosition().x, (int)rightBoundary->GetPosition().y + j, (int)rightBoundary->GetPosition().z)->isObstacle = true;
		grid->GetNodeAt((int)leftBoundary->GetPosition().x, (int)leftBoundary->GetPosition().y - j, (int)leftBoundary->GetPosition().z)->isObstacle = true;
		grid->GetNodeAt((int)rightBoundary->GetPosition().x, (int)rightBoundary->GetPosition().y - j, (int)rightBoundary->GetPosition().z)->isObstacle = true;

		grid->GetNodeAt((int)topBoundary->GetPosition().x + j, (int)topBoundary->GetPosition().y, (int)topBoundary->GetPosition().z)->isObstacle = true;
		grid->GetNodeAt((int)bottomBoundary->GetPosition().x + j, (int)bottomBoundary->GetPosition().y, (int)bottomBoundary->GetPosition().z)->isObstacle = true;
		grid->GetNodeAt((int)topBoundary->GetPosition().x - j, (int)topBoundary->GetPosition().y, (int)topBoundary->GetPosition().z)->isObstacle = true;
		grid->GetNodeAt((int)bottomBoundary->GetPosition().x - j, (int)bottomBoundary->GetPosition().y, (int)bottomBoundary->GetPosition().z)->isObstacle = true;
	}

	activeGameObjects.push_back(leftBoundary);
	activeGameObjects.push_back(rightBoundary);
	activeGameObjects.push_back(topBoundary);
	activeGameObjects.push_back(bottomBoundary);

	for (int i = 0; i < 3; ++i) {

		Assets::GameObject* verticalwall = new Assets::GameObject(MeshEffectPairs[5].mesh, MeshEffectPairs[2].effect, Math::sVector(110.0f - (i * 15), 88.0f + (i * 15), 100.0f), Math::sVector(0.5f, 3.0f, 0.5f));
		Assets::GameObject* horizontalwall = new Assets::GameObject(MeshEffectPairs[4].mesh, MeshEffectPairs[2].effect, Math::sVector(80.0f + (i * 14), 85.0f + (i  * 15), 100.0f), Math::sVector(5.0f, 0.5f, 0.5f));

		verticalwall->GetCollider()->SetCenterOffset(Math::sVector(-1.0f, 0.0f, 0.0f));
		horizontalwall->GetCollider()->SetCenterOffset(Math::sVector(-1.0f, 0.0f, 0.0f));

		activeGameObjects.push_back(horizontalwall);
		activeGameObjects.push_back(verticalwall);

		for (int j = 0; j < 4; ++j) {

			grid->GetNodeAt((int)verticalwall->GetPosition().x, (int)verticalwall->GetPosition().y + j, (int)verticalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)verticalwall->GetPosition().x, (int)verticalwall->GetPosition().y - j, (int)verticalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)verticalwall->GetPosition().x + 1, (int)verticalwall->GetPosition().y + j, (int)verticalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)verticalwall->GetPosition().x + 1, (int)verticalwall->GetPosition().y - j, (int)verticalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)verticalwall->GetPosition().x - 1, (int)verticalwall->GetPosition().y + j, (int)verticalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)verticalwall->GetPosition().x - 1, (int)verticalwall->GetPosition().y - j, (int)verticalwall->GetPosition().z)->isObstacle = true;
		}

		for (int j = 0; j < 6; ++j) {

			grid->GetNodeAt((int)horizontalwall->GetPosition().x + j, (int)horizontalwall->GetPosition().y, (int)horizontalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)horizontalwall->GetPosition().x - j, (int)horizontalwall->GetPosition().y, (int)horizontalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)horizontalwall->GetPosition().x + j, (int)horizontalwall->GetPosition().y + 1, (int)horizontalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)horizontalwall->GetPosition().x - j, (int)horizontalwall->GetPosition().y + 1, (int)horizontalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)horizontalwall->GetPosition().x + j, (int)horizontalwall->GetPosition().y - 1, (int)horizontalwall->GetPosition().z)->isObstacle = true;
			grid->GetNodeAt((int)horizontalwall->GetPosition().x - j, (int)horizontalwall->GetPosition().y - 1, (int)horizontalwall->GetPosition().z)->isObstacle = true;
		}
	}

	rotator = new Assets::GameObject(MeshEffectPairs[5].mesh, MeshEffectPairs[1].effect, Math::sVector(100.0f, 88.0f, 100.0f), Math::sVector(0.5f, 3.0f, 0.5f));
	rotator->GetCollider()->SetCenterOffset(Math::sVector(-1.0f, 0.0f, 0.0f));
	activeGameObjects.push_back(rotator);

	path = PathFinding::GetPath(PathFinding::Grid::GetGrid()->GetNodeAt((int)npc->GetPosition().x, (int)npc->GetPosition().y, (int)npc->GetPosition().z), PathFinding::Grid::GetGrid()->GetNodeAt((int)player->GetPosition().x, (int)player->GetPosition().y, (int)player->GetPosition().z));
	currentNPCTargetLoc = Math::sVector(path.back()->x, path.back()->y, path.back()->z);
	path.pop_back();

	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	while (!activeGameObjects.empty())
	{
		Assets::GameObject* currObj = activeGameObjects.back();
		activeGameObjects.pop_back();
		delete currObj;
	}

	for (unsigned int i = 0; i < numMeshEffectPairs; ++i)
	{
		if (MeshEffectPairs[i].mesh) {
		
			MeshEffectPairs[i].mesh->DecrementReferenceCount();
			MeshEffectPairs[i].mesh = nullptr;
		}

		if (MeshEffectPairs[i].effect) {

			MeshEffectPairs[i].effect->DecrementReferenceCount();
			MeshEffectPairs[i].effect = nullptr;
		}
	}

	Logging::CleanUp();

	Logging::OutputMessage("\nFinalGame - CleanUp Successful ----------- FuncName - %s, File - %s, Line - %d\n", __func__, __FILE__, __LINE__);

	return Results::Success;
}

void eae6320::cFinalGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
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

	Graphics::SetBackgroundColor(Graphics::Green);

	Graphics::SetRenderableObjects(renderableObjects, (uint16_t) activeGameObjects.size());

	Graphics::SetCameraTransforms(camera.CreateWorldToCameraTransform(i_elapsedSecondCount_sinceLastSimulationUpdate), camera.CreateCameraToProjectedTransform_perspective());
}
