#include "precompiled.h"
#include "MeshComponent.h"


MeshComponent::MeshComponent(Entity* entity, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& glslPath)
	: Component(entity), Buffers(vertices, indices), Shader(glslPath)
{

}
