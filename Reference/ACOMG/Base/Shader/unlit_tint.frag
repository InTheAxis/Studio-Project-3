#version 330 core

//unlit.frag but multiply with tintColor

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec4 fragmentColor;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;

// Ouput data
out vec4 color;

// Constant values
const int MAX_TEXTURES = 8;

//misc uniforms
uniform bool colorTextureEnabled[MAX_TEXTURES];
uniform sampler2D colorTexture[MAX_TEXTURES];
uniform vec4 tintColor;

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
	
	color = baseColor * tintColor;

	if (color.a < 0.0001)
		discard;
}