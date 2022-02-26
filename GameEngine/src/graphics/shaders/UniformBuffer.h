#pragma once

/*

struct Uniform
{
	std::string Name;
	int Offset;
	int Size;

	std::vector<char> Data;

	Uniform(const std::string& name, const int& offset, const int& size)
	{
		Name = name;
		Offset = offset;
		Size = size;
		Data = std::vector<char>(size);
	}

	Uniform(const std::string& name, const int& offset, const std::vector<char>& data)
	{
		Name = name;
		Offset = offset;
		Size = data.size();
		Data = data;
	}
};

class UniformBuffer
{
private:
	std::string m_Name;
	int m_Binding;
	int m_Size;
	unsigned int m_GLID;

	std::vector<Uniform> m_Uniforms;

public:
	UniformBuffer(const std::string& name, const int& binding, const std::vector<Uniform>& uniforms);

	void SetData(const std::string& uniformName, const std::vector<char>& data);

	void Delete();
	void Bind();

private:
	void openglUpdateData();
};

*/

