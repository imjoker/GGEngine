--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/sample.shader", arguments = { "fragment" } },		

		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/sample2.shader", arguments = { "fragment" } },	

		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/sample3.shader", arguments = { "fragment" } },	
	},
	meshes =
	{
		"Meshes/ball.mesh", "Meshes/cone.mesh", "Meshes/floor.mesh", "Meshes/ring.mesh", "Meshes/wall.mesh", "Meshes/gear.mesh", "Meshes/horizontal.mesh", "Meshes/vertical.mesh", "Meshes/horizontalwall.mesh", "Meshes/verticalwall.mesh",
	},
}
