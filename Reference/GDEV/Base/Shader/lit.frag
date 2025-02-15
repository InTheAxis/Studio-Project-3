#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec4 fragmentColor;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;

// Ouput data
out vec4 color;

struct Light 
{
	bool enabled;
	int type; //0 point, 1 dir, 2 spot
	vec3 position_cameraspace;
	vec4 color;
	float power, kC, kL, kQ;
	vec3 spotDirection;
	float cosCutoff, cosInner;
	float exponent;
};

struct Material 
{
	vec3 kAmbient, kDiffuse, kSpecular;
	float kShininess;
};

struct Fog
{
	bool enabled;
	vec4 color;
	float start, end, density;
	int type; //0 linear, 1 exp, 2 expsq
};


// constants
const int MAX_LIGHTS = 8;
const int MAX_TEXTURES = 8;

// struct uniforms
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform Fog fog;

// misc uniforms
uniform int numLights;
uniform bool colorTextureEnabled[MAX_TEXTURES];
uniform sampler2D colorTexture[MAX_TEXTURES];

//function declarations
float GetAttenuation(Light light, float dist);
float GetSpotlightEffect(Light light, vec3 lightDirection);
float GetFogFactor(Fog fog, float fogDist);

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

	/**HANDLING LIGHTS**/

	vec3 eyeDirection_cameraspace = -vertexPosition_cameraspace;
	vec3 E = normalize(eyeDirection_cameraspace);
	vec3 N = normalize(vertexNormal_cameraspace);

	vec4 lightColor = color * (material.kAmbient, 1);

	//applying light diffuse and specular
	for(int i = 0; i < numLights; ++i)
	{
		if (!lights[i].enabled)
			continue;
		// Light direction
		float spotlightEffect = 1;	
		vec3 lightDirection_cameraspace;
		if(lights[i].type == 1) 
		{
			lightDirection_cameraspace = lights[i].position_cameraspace;
		}
		else if(lights[i].type == 2) 
		{
			lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
			spotlightEffect = GetSpotlightEffect(lights[i], lightDirection_cameraspace);
		}
		else 
		{
			lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
		}
		// Distance to the light
		float dist = length(lightDirection_cameraspace);
		
		// Light attenuation
		float attenuationFactor = GetAttenuation(lights[i], dist);

		vec3 L = normalize(lightDirection_cameraspace);
		float cosTheta = clamp(dot(N, L), 0, 1);		
		vec3 R = reflect(-L, N);
		float cosAlpha = clamp(dot(E, R), 0, 1);
		
		//preserve alpha
		float tempAlpha = baseColor.w;

		lightColor +=
			//apply diffuse
			baseColor * vec4(material.kDiffuse, 1) * lights[i].color * lights[i].power * cosTheta * attenuationFactor * spotlightEffect +		
			//apply specular
			vec4(material.kSpecular, 1) * lights[i].color * lights[i].power * pow(cosAlpha, material.kShininess) * attenuationFactor * spotlightEffect;
		
		lightColor.w = tempAlpha;
		color = lightColor;
	}

	/**HANDLING FOG**/
	if (fog.enabled)
	{		
		float fogDist = length(gl_FragCoord.xyz - vertexPosition_cameraspace);
		float fogFactor = GetFogFactor(fog, fogDist);
		color.xyz = mix(color.xyz, fog.color.xyz, fogFactor);
	}
}


//function definitions

float GetAttenuation(Light light, float dist) 
{
	if(light.type != 0)
		return 1;
	else
		return 1 / max(1, light.kC + light.kL * dist + light.kQ * dist * dist);
}

float GetSpotlightEffect(Light light, vec3 lightDirection) 
{
	vec3 S = normalize(light.spotDirection);
	vec3 L = normalize(lightDirection);
	float cosDirection = dot(L, S);
	//return smoothstep(light.cosCutoff, light.cosInner, cosDirection);
	if(cosDirection < light.cosCutoff)
		return 0;
	else
		return 1;//pow(cosDirection, light.exponent);
}

float GetFogFactor(Fog fog, float fogDist)
{
	if (!fog.enabled)
		return 0.f;

	float factor = 0.f;

	if (fog.type == 1)
		factor = exp(-fog.density * fogDist);
	else if (fog.type == 2)
		factor = exp(-pow(fog.density * fogDist, 2));
	else
		factor = (fog.end - fogDist) / (fog.end - fog.start);

	factor = clamp(factor, 0.f, 1.f);
	return 1 - factor;
}