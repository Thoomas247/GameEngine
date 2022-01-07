#ifndef MESH
#define MESH

#include <vector>

#include "glm/glm.hpp"

/*
	Mesh class
	Base class for anything which needs to be rendered
	on screen
	Should not be inherited from
*/

#include "../core/GameObject.h"
#include "../structs/RenderData.h"
#include "MeshData.h"
#include "Skeleton.h"

class Mesh : public GameObject
{
private:
	RenderData m_RenderData;	// in a struct so it can easily be passed to the renderer through the MeshData each frame
	std::shared_ptr<MeshData> m_MeshData;	// many mesh objects can have the same MeshData
	std::shared_ptr<Skeleton> m_Skeleton;	// many mesh objects can have the same Skeleton

public:
	Mesh();
	Mesh(const RenderData& renderData, std::shared_ptr<MeshData> meshData, std::shared_ptr<Skeleton> skeleton, glm::mat4 transform = glm::mat4(1.0f));

private:
	void onUpdate(const float& deltaTime) override;
};

#endif // !MESH
