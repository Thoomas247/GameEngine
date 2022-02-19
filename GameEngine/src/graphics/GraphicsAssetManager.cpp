#include "precompiled.h"
#include "GraphicsAssetManager.h"

const std::vector<Vertex> GraphicsAssetManager::c_SquareVertices = { Vertex(glm::vec3(0.0f, 0.0f, -1.0f)), Vertex(glm::vec3(0.0f, 1.0f, -1.0f)), Vertex(glm::vec3(1.0f, 0.0f, 1.0f)), Vertex(glm::vec3(1.0f, 1.0f, 1.0f)) };
const std::vector<unsigned int> GraphicsAssetManager::c_SquareIndices = { 0, 2, 1, 1, 2, 3 };

std::map<std::string, unsigned int> GraphicsAssetManager::s_UploadedTextureIDs;


/* -- PUBLIC -- */

/// <summary>
/// Prepares all of the default textures and shaders.
/// Required for proper rendering.
/// </summary>
void GraphicsAssetManager::Init()
{
	// create default 1x1 pixel full-white texture
	unsigned int glID = 0;
	int width = 1;
	int height = 1;

	glCreateTextures(GL_TEXTURE_2D, 1, &glID);
	glTextureStorage2D(glID, 1, GL_RGB8, width, height);

	GLubyte data[] = { 255, 255, 255, 255 };
	glTextureSubImage2D(glID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	s_UploadedTextureIDs["Default"] = glID;
}

unsigned int GraphicsAssetManager::LoadTexture(const std::string& absolutePath)
{
	if (absolutePath == "")
	{
		return s_UploadedTextureIDs["Default"];
	}

	auto it = s_UploadedTextureIDs.find(absolutePath);

	if (it != s_UploadedTextureIDs.end())
	{
		return it->second;
	}

	unsigned int glID;
	int width;
	int height;
	int channels;

	// create opengl texture
	glCreateTextures(GL_TEXTURE_2D, 1, &glID);

	glTextureParameteri(glID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(glID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameteri(glID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(glID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	unsigned char* data = stbi_load(absolutePath.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		GLenum format = GL_RED;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;
		else
		{
			LOG_ERROR("ASSET_MANAGER::Texture format not supported!");
		}

		glTextureStorage2D(glID, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(glID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(glID);
	}
	else
	{
		LOG_ERROR("ASSET_MANAGER::Failed to load texture at path " + absolutePath + "! If trying to load the default texture, make sure GraphicsAssetManager has been initialized.");
	}

	stbi_image_free(data);

	// cache the ID
	s_UploadedTextureIDs[absolutePath] = glID;

	return glID;
}
/*
ShaderAsset GraphicsAssetManager::LoadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = shaderLoadFileContents(vertexPath).c_str();
	std::string fragmentString = shaderLoadFileContents(fragmentPath).c_str();

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	// compile shaders
	unsigned int vertex, fragment;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	shaderCheckCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	shaderCheckCompileErrors(fragment, "FRAGMENT");

	// shader Program
	unsigned int glID = glCreateProgram();
	glAttachShader(glID, vertex);
	glAttachShader(glID, fragment);
	glLinkProgram(glID);
	shaderCheckCompileErrors(glID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	auto pos = vertexPath.find_last_of("/") + 1;

	std::string name;
	if (pos != vertexPath.npos)
	{
		name = vertexPath.substr(pos);
	}
	else
	{
		name = vertexPath;
	}

	return ShaderAsset(name, vertexPath, glID);
}
*/

VertexArray GraphicsAssetManager::LoadVertexArray(const std::vector<Vertex> vertices, const std::vector<unsigned int>& indices)
{
	unsigned int VBO;
	unsigned int EBO;
	unsigned int glID;

	glCreateVertexArrays(1, &glID);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glBindVertexArray(glID);

	glNamedBufferData(VBO, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glNamedBufferData(EBO, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(glID, 0);	// vertex positions attrib
	glVertexAttribBinding(0, 0);
	glVertexArrayAttribFormat(glID, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(glID, 1);	// vertex normals attrib
	glVertexAttribBinding(1, 0);
	glVertexArrayAttribFormat(glID, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));

	glEnableVertexArrayAttrib(glID, 2);	// vertex texcoords attrib
	glVertexAttribBinding(2, 0);
	glVertexArrayAttribFormat(glID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoord));

	glVertexArrayVertexBuffer(glID, 0, VBO, 0, sizeof(Vertex));	// bind VBO to VAO
	glVertexArrayElementBuffer(glID, EBO);	// bind EBO to VAO

	return VertexArray(glID, indices.size());
}


/* -- PRIVATE -- */

void GraphicsAssetManager::shaderCheckCompileErrors(const unsigned int& shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Shader compilation failed!" + std::string(" - " + type + ": " + infoLog));
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Shader linking failed!" + std::string(" - " + type + ": " + infoLog));
		}
	}
}

std::string GraphicsAssetManager::shaderLoadFileContents(const std::string& absolutePath)
{
	std::string fileContents;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fileStream.open(absolutePath);

		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();

		fileStream.close();

		fileContents = stringStream.str();
	}
	catch (std::ifstream::failure&) {
		LOG_ERROR("SHADER::File at path " + absolutePath + " not successfully read. If trying to load the default texture, make sure GraphicsAssetManager has been initialized.");
	}

	return fileContents;
}