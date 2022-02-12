#pragma once
#include "GraphicsAsset.h"

#include "../../core/Log.h"

class TextureAsset : public GraphicsAsset
{
private:
	int m_Width;
	int m_Height;
	int m_Channels;

public:
	/// <summary>
	/// Default constructor should never be used.
	/// </summary>
	TextureAsset()
		: GraphicsAsset("Invalid", "Invalid", 0)
	{
		m_Width = 0;
		m_Height = 0;
		m_Channels = 0;
	}
	TextureAsset(const std::string& name, const std::string& path, const unsigned int& glID, const int& width, const int& height, const int& channels)
		: GraphicsAsset(name, path, glID)
	{
		m_Width = width;
		m_Height = height;
		m_Channels = channels;
	}

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	int GetChannels() { return m_Channels; }
};