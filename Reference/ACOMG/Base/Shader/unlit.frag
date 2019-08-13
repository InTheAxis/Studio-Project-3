#version 330 core

//lit.frag but without light and fog stuff

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec4 fragmentColor;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;

// Ouput data
out vec4 color;

// constants
const int MAX_TEXTURES = 8;

// misc uniforms
uniform bool colorTextureEnabled[MAX_TEXTURES];
uniform sampler2D colorTexture[MAX_TEXTURES];

void main()
{
	/**HANDLING TEXTURES**/

	vec4 baseColor = vec4(0);
	//applying textures
	int texCount = 0;
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if(colorTextureEnabled[i] == true)
		{
			baseColor += texture2D( colorTexture[i], texCoord );
			++texCount;
		}
	}
	if (texCount > 0)
		baseColor = baseColor / texCount; //TODO add weights
	else
		baseColor = fragmentColor;
	
	color = baseColor;

	if (color.a < 0.0001)
		discard;
}