#version 330 core

//lit.frag but without light and fog stuff

// Interpolated values from the vertex shaders
in vec4 fragColor;
in vec3 fragPos;
in vec2 texCoord;
in vec3 vertexPos_cameraspace;
in vec3 vertNormal_cameraspace;

// Ouput data
out vec4 color;

// constants
const int MAX_COLORMAPS = 13;

// misc uniforms
struct Material
{
	vec4 albedo;

	bool colorMapEnabled[MAX_COLORMAPS];
	sampler2D colorMap[MAX_COLORMAPS];
};
uniform Material material;
uniform mat4 model;

void main()
{
	/**HANDLING TEXTURES**/
	vec4 baseColor = vec4(0);
	int colorCount = 0;
	for (int i = 0; i < MAX_COLORMAPS; ++i)
	{
		if (material.colorMapEnabled[i])
		{
			baseColor += texture2D(material.colorMap[i], texCoord);
			++colorCount;
		}
	}

	if (colorCount > 0)
		baseColor = baseColor / colorCount;
	else
		baseColor = fragColor;
	
	color = material.albedo * baseColor;

	if (color.a < 0.01) 
		discard;

	/**HANDLING LIGHT**/
	vec3 pos = vec3(model[3][0],model[3][1],model[3][2]);
	vec3 lightDir = normalize(vec3(1,1,0)) * 0.5;
	vec3 lightPos = pos + lightDir;

	if (length(lightPos.xy - fragPos.xy) > 0.7 * model[2][2])
	{
		color.rgb *= 0.5;
	}

}