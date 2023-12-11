/*
	This class is your specific game
*/

#ifndef EAE6320_CEXAMPLEGAME_H
#define EAE6320_CEXAMPLEGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>
#include <Engine/Math/sVector.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Assets/Camera.h>
#include <Engine/Assets/GameObject.h>
#include <Plugin/PathFinding/PathFinding.h>
#include <Plugin/PathFinding/Grid.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif

// Class Declaration
//==================

namespace eae6320
{
	class Player final : public Assets::GameObject
	{
	public:

		Player(Graphics::VertexMesh* pMesh, Graphics::cEffect* pEffect, Math::sVector pPosition, Math::sVector pColliderExtents) : Assets::GameObject(pMesh, pEffect, pPosition, pColliderExtents) {}
		~Player () {}

		void OnCollisionEnter(ChrisZ::Physics::Collider* other) override { if (other->GetGameObject()->GetRigidBody()->GetAngularVelocity() == Math::sVector()) isDead = true; }

		bool isDead = false;
	};

	class cFinalGame final : public Application::iApplication
	{
		// Inherited Implementation
		//=========================

	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "Yesh's EAE6320 Final Game"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
			;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "Yesh's EAE6320 Example Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_EAEALIEN; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_EAEALIEN; return &iconId_small; }
#endif

		// Run
		//----

		void UpdateBasedOnInput() final;
		void UpdateSimulationBasedOnInput() final;
		void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) final;

		// Initialize / Clean Up
		//----------------------

		cResult Initialize() final;
		cResult CleanUp() final;

		// 
		virtual void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate) override;

	// Data members
		
		// Mesh Effect Data
		//--------------
		static constexpr uint16_t					numMeshEffectPairs = 6;

		eae6320::Graphics::MeshEffectData			MeshEffectPairs[numMeshEffectPairs];
		eae6320::Assets::Camera						camera{};
		std::vector<eae6320::Assets::GameObject*>	activeGameObjects;
		eae6320::Assets::GameObject*				player = nullptr;
		eae6320::Assets::GameObject*				npc = nullptr;
		eae6320::Assets::GameObject*				rotator;	
		Math::sVector								playerVelocity;
		Math::sVector								currentNPCTargetLoc;
		std::vector<PathFinding::Node*>				path;
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CEXAMPLEGAME_H
