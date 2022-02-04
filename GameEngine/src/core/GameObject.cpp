#include "GameObject.h"

//#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "Log.h"

GameObject::GameObject()
{
	m_Name = "GameObject";
}

// PUBLIC
GameObject::GameObject(const std::string& name)
{
	m_Name = name;
}

/// <summary>
/// Runs once when the game world initializes. 
/// Runs the onSetup() method defined by the game and then runs SetUp() on all this GameObject's children.
/// </summary>
void GameObject::SetUp()
{
	onSetUp();

	for (const auto& object : m_Children)
	{
		object->SetUp();
	}
}

/// <summary>
/// Runs every frame after the world has started.
/// Runs the onUpdate() method defined by the game and then runs Update() on all this GameObject's children.
/// </summary>
/// <param name="deltaTime"></param>
/// <param name="parentTransform"></param>
void GameObject::Update(const float& deltaTime, const glm::mat4& parentTransform)
{
	onUpdate(deltaTime);
	calcTransforms(parentTransform);

	for (const auto& object : m_Children)
	{
		object->Update(deltaTime, m_GlobalTransform);
	}
}

/// <summary>
/// Adds a child GameObject to this GameObject.
/// The child object will now move with the parent in the game world and be updated each frame.
/// </summary>
/// <param name="name"></param>
/// <param name="object"></param>
void GameObject::AddChild(const std::shared_ptr<GameObject>& object)
{
	m_Children.push_back(object);
}

// PRIVATE
void GameObject::calcTransforms(const glm::mat4& parentTransform)
{
	if (LocalPosition == m_LastLocalPosition && LocalRotation == m_LastLocalRotation && LocalScale == m_LastLocalScale)
	{
		m_GlobalTransform = parentTransform * m_LocalTransform;
		return;
	}

	m_LocalTransform = glm::translate(glm::mat4(1.0f), LocalPosition) * glm::mat4_cast(LocalRotation) * glm::scale(glm::mat4(1.0f), LocalScale);

	m_LastLocalPosition = LocalPosition;
	m_LastLocalRotation = LocalRotation;
	m_LastLocalScale = LocalScale;

	m_GlobalTransform = parentTransform * m_LocalTransform;
}

void GameObject::decomposeMatIntoComponents(const glm::mat4& matrix)
{
	// TODO: write own decompose function
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(matrix, LocalScale, LocalRotation, LocalPosition, skew, perspective);
}

void GameObject::onSetUp()
{
	// Does nothing by default
}

void GameObject::onUpdate(const float&)
{
	// Does nothing by default
}