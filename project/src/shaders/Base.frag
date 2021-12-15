#version 460 core

out vec4 FragColor;

// for color, if no textrue is present, assume texture is sRGB{1, 1, 1, 1} and multiply by COLOR_0 attribute of vertex

void main() {
	FragColor = vec4(1.0, 0.6, 0.3, 1.0);
}