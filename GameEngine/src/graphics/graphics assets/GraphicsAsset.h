#pragma once

/// <summary>
/// Represents any asset which is stored on the GPU.
/// </summary>
class GraphicsAsset
{
protected:
	uint64_t m_UUID;
	std::string m_Name;
	std::string m_Path;
	unsigned int m_GLID;

public:
	GraphicsAsset()
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "New Graphics Asset";
		m_Path = "Default";
		m_GLID = 0;
	}

	GraphicsAsset(const std::string& name, const std::string& path, const unsigned int& glID)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;
		m_Path = path;
		m_GLID = glID;
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string GetName() const { return m_Name; }
	const std::string GetPath() const { return m_Path; }
	const unsigned int GetGLID() const { return m_GLID; }
};