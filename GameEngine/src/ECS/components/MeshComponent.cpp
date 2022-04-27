#include "precompiled.h"
#include "MeshComponent.h"


/* -- PUBLIC -- */

MeshComponent::MeshComponent(Entity* entity, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& glslPath)
	: Component(entity)
{
	VertexBuffer.Init(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(vertices[0]) * vertices.size(), (void*)vertices.data());
	IndexBuffer.Init(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, sizeof(indices[0]) * indices.size(), (void*)indices.data());
	NumIndices = (uint32_t)indices.size();

	Shader.Compile(glslPath);

	m_Vertices = vertices;
	m_Indices = indices;
	m_GlslPath = glslPath;
}

MeshComponent::MeshComponent(Entity* entity, json j)
	: Component(entity)
{
	// vertices
	if (j["vertices"].is_null())
	{
		LOG_ERROR("MESH_COMPONENT::Mesh vertices cannot be null!");
	}

	std::vector<float> vertFloats = j["vertices"];
	std::vector<float>::iterator vectorBegin = vertFloats.begin();

	std::vector<float>::iterator thisVertBegin;
	std::vector<float>::iterator thisVertEnd;

	for (int i = 0; i < vertFloats.size(); i += Vertex::NUM_VERTEX_FLOATS)
	{
		thisVertBegin = vectorBegin + i;
		thisVertEnd = thisVertBegin + Vertex::NUM_VERTEX_FLOATS;
		m_Vertices.emplace_back(std::vector<float>(thisVertBegin, thisVertEnd));
	}

	VertexBuffer.Init(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(m_Vertices[0]) * m_Vertices.size(), (void*)m_Vertices.data());

	// indices
	if (j["indices"].is_null())
	{
		LOG_ERROR("MESH_COMPONENT::Mesh indices cannot be null!");
	}
	std::vector<uint32_t> indices = j["indices"];	// the json library requires this copy
	m_Indices = indices;

	IndexBuffer.Init(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, sizeof(m_Indices[0]) * m_Indices.size(), (void*)m_Indices.data());
	NumIndices = (uint32_t)m_Indices.size();

	// shader
	if (j["shaderPath"].is_null())
	{
		LOG_WARN("MESH_COMPONENT::Mesh has no shader, using the default one.");
		m_GlslPath = DEFAULT_SHADER_PATH;
		Shader.Compile(m_GlslPath);
	}
	else
	{
		m_GlslPath = j["shaderPath"];
		Shader.Compile(m_GlslPath);
	}
}

json MeshComponent::Serialize()
{
	json j;

	for (Vertex vert : m_Vertices)
	{
		j["vertices"].push_back(vert.Serialize());
	}

	j["indices"] = m_Indices;

	j["shaderPath"] = m_GlslPath;

	return j;
}
