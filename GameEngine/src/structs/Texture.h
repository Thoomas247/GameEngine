#pragma once

struct Texture
{
	unsigned int ID;
	int Width;
	int Height;
	int Channels;

	Texture()
	{
		ID = 0;
		Width = -1;
		Height = -1;
		Channels = -1;
	}
};