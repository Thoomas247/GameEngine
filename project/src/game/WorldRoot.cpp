#include "WorldRoot.h"

void WorldRoot::Update(Data& data, const float& deltaTime)
{
	Player.Update(data, deltaTime);

	for (auto& [key, mesh] : MeshChildren)
	{
		mesh.Update(data, deltaTime);
	}

}

void WorldRoot::AddChildMesh(const std::string& name, Mesh& mesh)
{
	MeshChildren[name] = mesh;
}
