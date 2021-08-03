#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture;
uniform sampler2D noiseTextureIndex;

struct PixelData 
{
		int colour_count;
		vec4[4] colours;
};

struct MaskData
{
		uint index;
		uint lifetime;
		uint behaviour;
		uint light;
};

uniform PixelData[10] u_Pixels;
uniform MaskData u_PixelMask;
uniform MaskData u_PixelBitOffset;

const float LIGHT_INVERVAL = 1.0 / 7.0;

void main()
{
		// FragColor = texture(ourTexture, TexCoord);
		uint value = uint(floatBitsToUint(texture2D(ourTexture, TexCoord).r));
		// int index = value & 0000000000001111;

		// No shift since start of Bits
		uint pixelType = (u_PixelMask.index & value);

		uint light = (u_PixelMask.light & value) >> u_PixelBitOffset.light;
		float addedLight = light * LIGHT_INVERVAL;

		int noiseIndex = int(floatBitsToUint(texture2D(noiseTextureIndex, TexCoord).r));

		FragColor = u_Pixels[pixelType].colours[noiseIndex % (u_Pixels[pixelType].colour_count)];
		if (addedLight != 0)
				BrightColor = vec4(vec3(FragColor), addedLight);
}