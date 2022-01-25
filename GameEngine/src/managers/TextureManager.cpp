#include "TextureManager.h"

#include "glad/gl.h"
#include "stb/stb_image.h"

#include "../core/Log.h"

std::map<std::string, std::shared_ptr<Texture>> TextureManager::TextureCache;

void TextureManager::Init()
{
	// create 1x1 pixel full-white texture
	Texture defaultTex;
	defaultTex.Width = 1;
	defaultTex.Height = 1;
	defaultTex.Channels = 3;

	glCreateTextures(GL_TEXTURE_2D, 1, &defaultTex.ID);
	glTextureStorage2D(defaultTex.ID, 1, GL_RGB8, defaultTex.Width, defaultTex.Height);

	GLubyte data[] = { 255, 255, 255, 255 };
	glTextureSubImage2D(defaultTex.ID, 0, 0, 0, defaultTex.Width, defaultTex.Height, GL_RGB, GL_UNSIGNED_BYTE, data);

	TextureCache["default"] = std::make_shared<Texture>(defaultTex);
}

std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& path)
{
	auto it = TextureManager::TextureCache.find(path);

	if (it != TextureManager::TextureCache.end())
	{
		return it->second;
	}

	if (path == "" || path.back() == '/')
	{
		return TextureCache["default"];	// TODO: make return default full white texture index
	}

	Texture texture;

	// create opengl texture
	glCreateTextures(GL_TEXTURE_2D, 1, &texture.ID);

	glTextureParameteri(texture.ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture.ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameteri(texture.ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(texture.ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	unsigned char* data = stbi_load(path.c_str(), &texture.Width, &texture.Height, &texture.Channels, 0);

	if (data)
	{
		GLenum format = GL_RED;
		if (texture.Channels == 1)
			format = GL_RED;
		else if (texture.Channels == 3)
			format = GL_RGB;
		else if (texture.Channels == 4)
			format = GL_RGBA;
		else
		{
			LOG_ERROR("TEXTURE_MANAGER::Texture format not supported!")
				return TextureCache["default"];
		}

		glTextureStorage2D(texture.ID, 1, GL_RGBA8, texture.Width, texture.Height);
		glTextureSubImage2D(texture.ID, 0, 0, 0, texture.Width, texture.Height, format, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(texture.ID);
	}
	else
	{
		LOG_ERROR("TEXTURE_MANAGER::Failed to load texture!")
		return TextureCache["default"];
	}

	stbi_image_free(data);

	TextureManager::TextureCache[path] = std::make_shared<Texture>(texture);
	return LoadTexture(path);
}
