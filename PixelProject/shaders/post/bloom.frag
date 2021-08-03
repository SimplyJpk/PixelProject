#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform bool bloom;
uniform vec3 ambientLight;

uniform int state;

void main()
{             
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    float bloomBrightness = texture(bloomBlur, TexCoords).a;
    vec3 result;
    if(bloom)
    {
        hdrColor += bloomColor * bloomBrightness; // additive blending
        result = (hdrColor * 0.65) + ambientLight;
    }
    FragColor = vec4(result, 1.0);

    // We don't really need HDR
    // // tone mapping
    // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));

    // Some harsh cutoff
    // if (bloomColor.r <= 0.15) {
    //     // result = vec3(0.0,0.0,0.0);
    //     result *= bloomColor.r;
    // }

    // See the different textures
    if (state == 0)
        FragColor = vec4(result, 1.0);
    else if (state == 1)
        FragColor = vec4(hdrColor, 1.0);
    else if (state == 2)
        FragColor = vec4(bloomColor, 1.0);

}