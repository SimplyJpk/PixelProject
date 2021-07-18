#version 410
const float PI = 3.1415926535;

in vec2 fTexCoord;

out vec4 fragColor;

uniform sampler2D target;

uniform int KernalState;

vec4 Simple()
{
	return texture(target, fTexCoord);
}

vec4 BoxBlur()
{
	vec2 texel = 1.0 / textureSize(target, 0).xy;

	// 9 Tap box Kernal
	vec4 colour = texture(target, fTexCoord);
	colour += texture(target, fTexCoord + vec2(-texel.x, texel.y));
	colour += texture(target, fTexCoord + vec2(-texel.x, 0));
	colour += texture(target, fTexCoord + vec2(-texel.x, -texel.y));
	colour += texture(target, fTexCoord + vec2(0, texel.y));
	colour += texture(target, fTexCoord + vec2(0, -texel.y));
	colour += texture(target, fTexCoord + vec2(texel.x, texel.y));
	colour += texture(target, fTexCoord + vec2(texel.x, 0));
	colour += texture(target, fTexCoord + vec2(texel.x, -texel.y));
	return colour / 9;
}

vec4 Distort()
{
	vec2 mid = vec2(0.5f);
	float distanceFromCentre = distance(fTexCoord, mid);

	if (distanceFromCentre < 0)
		distanceFromCentre += 0.025;
	else
		distanceFromCentre -= 0.025;

	vec2 normalizedCoord = normalize(fTexCoord - mid);
	float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05;
	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(target, newCoord);
}

vec4 GreyScale()
{
	vec4 colour = texture(target, fTexCoord);
	float max = colour.x + colour.y + colour.z;
	max = max / 3;
	colour = vec4(max, max, max, colour.a);
	return colour;
}

vec4 Negative()
{
	return vec4(1.0, 1.0, 1.0, 1.0) - texture(target, fTexCoord);
}

vec4 MoreBlur()
{
	vec2 texel = 1.0 / textureSize(target, 0).xy;

	vec4 sum = vec4(0.0);
	for (int x = -4; x <= 4; x++)
		for (int y = -4; y <= 4; y++)
			sum += texture(target, vec2(fTexCoord.x + x * texel.y, fTexCoord.y + y * texel.x)) / 81.0;
	return sum;
}

vec4 Sobel()
{
	vec2 texel = 1.0 / textureSize(target, 0).xy;

	vec4 top = texture(target, vec2(fTexCoord.x, fTexCoord.y + texel.y));
	vec4 bottom = texture(target, vec2(fTexCoord.x, fTexCoord.y - texel.y));
	vec4 left = texture(target, vec2(fTexCoord.x - texel.x, fTexCoord.y));
	vec4 right = texture(target, vec2(fTexCoord.x + texel.x, fTexCoord.y));
	vec4 topLeft = texture(target, vec2(fTexCoord.x - texel.x, fTexCoord.y + texel.y));
	vec4 topRight = texture(target, vec2(fTexCoord.x + texel.x, fTexCoord.y + texel.y));
	vec4 bottomLeft = texture(target, vec2(fTexCoord.x - texel.x, fTexCoord.y - texel.y));
	vec4 bottomRight = texture(target, vec2(fTexCoord.x + texel.x, fTexCoord.y - texel.y));
	vec4 sx = -topLeft - 2 * left - bottomLeft + topRight + 2 * right + bottomRight;
	vec4 sy = -topLeft - 2 * top - topRight + bottomLeft + 2 * bottom + bottomRight;
	vec4 sobel = sqrt(sx * sx + sy * sy);
	return sobel;
}
void main()
{
	vec4 colour = vec4(0, 0, 0, 1);
	int kernalStage = KernalState;
	int stateChange = 0;
	// Not a good way to do this, surely there is a better way?
	while (kernalStage > 0)
	{
		if (kernalStage >= 64)
		{
			colour += Distort();
			kernalStage -= 64;
			stateChange++;
		}
		if (kernalStage >= 32)
		{
			colour += MoreBlur();
			kernalStage -= 32;
			stateChange++;
		}
		if (kernalStage >= 16)
		{
			colour += Negative();
			kernalStage -= 16;
			stateChange++;
		}
		if (kernalStage >= 8)
		{
			colour += Sobel();
			kernalStage -= 8;
			stateChange++;
		}
		if (kernalStage >= 4)
		{
			colour += GreyScale();
			kernalStage -= 4;
			stateChange++;
		}
		if (kernalStage >= 2)
		{
			colour += BoxBlur();
			kernalStage -= 2;
			stateChange++;
		}
		if (kernalStage >= 1)
		{
			colour += Simple();
			kernalStage -= 1;
			stateChange++;
		}
	}

	// Crap Bitmask, not even bitmask
	// Crap Bitmask, not even bitmask
	// Crap Bitmask, not even bitmask

	// if (colour == vec4(0, 0, 0, 1))
	// 	discard;

	fragColor = vec4(colour.xyz / stateChange, 1);
}