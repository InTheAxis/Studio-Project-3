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
const int MAX_COLORMAPS = 11;

// misc uniforms
struct Material
{
	vec4 albedo;

	bool colorMapEnabled[MAX_COLORMAPS];
	sampler2D colorMap[MAX_COLORMAPS];
};
uniform Material material;
struct HSV
{
	float h;
	float s;
	float v;
};
uniform HSV hsv;

vec3 rgbToHsv(vec3 rgb);
vec3 hsvToRgb(vec3 hsv);

void main()
{
	/**HANDLING TEXTURES**/
	vec4 baseColor = vec4(0);
//	int colorCount = 0;
//	for (int i = 0; i < MAX_COLORMAPS; ++i)
//	{
//		if (material.colorMapEnabled[i])
//		{
//			baseColor += texture2D(material.colorMap[i], texCoord);
//			++colorCount;
//		}
//	}

	if (material.colorMapEnabled[0])
		baseColor = texture2D(material.colorMap[0], texCoord);
	else
		baseColor = fragColor;
	
	color = material.albedo * baseColor;
	
	//apply hsv changes
	vec3 targetHsv = rgbToHsv(vec3(color));	
	if (hsv.h >= 0)
		targetHsv.x = hsv.h;
	if (hsv.s >= 0 && targetHsv.y > 0.3)
		targetHsv.y = clamp(hsv.s, 0, 1);
	if (hsv.v >= 0)
		targetHsv.z = clamp(hsv.v, 0, 1);
	vec3 newColor = hsvToRgb(targetHsv);

	color.rgb = newColor;

	if (color.a < 0.01) 
		discard;
}




vec3 rgbToHsv(vec3 rgb)
{
	vec3 ret; //hue, sat, val - x,y,z
	float m, chroma;

	m = max(max(rgb.r, rgb.g), rgb.b);	
	chroma = m - min(min(rgb.r, rgb.g), rgb.b);

	if(chroma > 0) 
	{
		if(m == rgb.r) 		
			ret.x = mod((rgb.g - rgb.b) / (chroma), 6);
		else if(m == rgb.g) 
			ret.x = (rgb.b - rgb.r) / (chroma) + 2;
		else if(m == rgb.b)
			ret.x = (rgb.r - rgb.g) / (chroma) + 4;
		ret.x *= 60;

		if(m > 0) 	
			ret.y = chroma / m;    
		else 
			ret.y = 0;    
		
		ret.z = m;
	}
	else 
	{
		ret.x = 0;
		ret.y = 0;
		ret.z = m;
	}
  
	if(ret.x < 0)   
		ret.x += 360;  

	return ret;
}

vec3 hsvToRgb(vec3 hsv)
{
	vec3 ret;	 
	float hPrime, m, x, chroma;

	chroma = hsv.z * hsv.y;
	hPrime = mod(hsv.x / 60, 6);
	x = chroma * (1 - abs(mod(hPrime, 2) - 1));
	m = hsv.z - chroma;

	if (hPrime >= 0 && hPrime <= 1)
		ret = vec3(chroma, x, 0);
	else if (hPrime > 1 && hPrime <= 2)
		ret = vec3(x, chroma, 0);
	else if (hPrime > 2 && hPrime <= 3)
		ret = vec3(0, chroma, x);
	else if (hPrime > 3 && hPrime <= 4)
		ret = vec3(0, x, chroma);
	else if (hPrime > 4 && hPrime <= 5)
		ret = vec3(x, 0, chroma);
	else if (hPrime > 5 && hPrime <= 6)
		ret = vec3(chroma, 0, x);
	else
		ret = vec3(0);

	ret += vec3(m,m,m);

	return ret;
}