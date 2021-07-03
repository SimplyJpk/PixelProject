#version 430 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture;
uniform sampler2D textureIndex;

struct PixelData 
{
		int colour_count;
		vec4[4] colours;
};

uniform PixelData[10] u_Pixels;

void main()
{
		// FragColor = texture(ourTexture, TexCoord);
		int value = int(floatBitsToUint(texture2D(ourTexture, TexCoord).r));
		// int index = value & 0000000000001111;

		int noiseIndex = int(floatBitsToUint(texture2D(textureIndex, TexCoord).r));

		FragColor = u_Pixels[value].colours[noiseIndex % (u_Pixels[value].colour_count)];
}