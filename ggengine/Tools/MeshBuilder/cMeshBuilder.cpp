#include "cMeshBuilder.h"

#include <Engine\Platform\Platform.h>
#include <Tools/AssetBuildLibrary/Functions.h>

using namespace eae6320;

cResult Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	std::string errorMessage;

	cResult result = eae6320::Platform::CopyFile(m_path_source, m_path_target, false, true, &errorMessage);

	if (result != Results::Success)
		OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());

	return result;
}
