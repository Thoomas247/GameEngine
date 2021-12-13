#ifndef WORLD_ROOT
#define WORLD_ROOT

#include <map>
#include <string>

#include "../core/Data.h"
#include "../renderer/Mesh.h"
#include "Player.h"

class WorldRoot
{
private:
	Player Player;
	std::map<std::string, Mesh> MeshChildren;

public:
	void Update(Data& data, const float& deltaTime);

	void AddChildMesh(const std::string& name, Mesh& mesh);
};


#endif // !WORLD_ROOT

