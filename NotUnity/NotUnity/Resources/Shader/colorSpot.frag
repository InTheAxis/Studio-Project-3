#version 330 core

// Interpolated values from the vertex shaders
in vec4 fragColor;
in vec2 texCoord;
in vec3 vertPos_modelSpace;

// Ouput data
out vec4 color;

// constants
const int MAX_POINTS = 2;

// misc uniforms
struct Material
{
	vec4 albedo;

	bool colorMapEnabled[1];
	sampler2D colorMap[1];
};
struct Point
{
	vec3 pos;
	float radius;
};

uniform Material material;
uniform Point pt[MAX_POINTS];
uniform mat4 view, proj;

float GetSaturation();
vec3 rgbToHsv(vec3 rgb);
vec3 hsvToRgb(vec3 hsv);

void main()
{
	/**HANDLING TEXTURES**/
	vec4 baseColor = vec4(0);
	if (material.colorMapEnabled[0])
		baseColor = texture2D(material.colorMap[0], texCoord);
	else
		baseColor = fragColor;
	
	color = material.albedo * baseColor;

	//apply hsv changes
	vec3 targetHsv = rgbToHsv(vec3(color));	
	if (targetHsv.y > 0.1)
	{
		targetHsv.y = GetSaturation();
		vec3 newColor = hsvToRgb(targetHsv);
		color.rgb = newColor;	
	} 

	if (color.a < 0.01) 
		discard;
}



float GetSaturation()
{	
	float sat = 0; //return value
	vec4 pointPos_screenSpace, vertPos_worldSpace; //storage for pos in their resp spaces
	bool inRadius = false; //whether its within any colorspots
	float smallestRatio = 1; //for "attenuation"
	for (int i = 0; i < MAX_POINTS; ++i)
	{
		if (pt[i].pos.z <= 0)
			break;

		//get the points position in screen space, for depth info, only useful for perspective
		pointPos_screenSpace = proj * view * vec4(pt[i].pos.xyz, 1);
		pointPos_screenSpace.xyz /= pointPos_screenSpace.w;
		//project the screen space vertPos back to world space
		vertPos_worldSpace =  inverse(proj * view) * vec4(vertPos_modelSpace.xy, pointPos_screenSpace.z, 1);
		vertPos_worldSpace.xyz /= vertPos_worldSpace.w;
		//find length
		float len = length(vertPos_worldSpace.xy - pt[i].pos.xy);
		//get smallest ratio & set the inRadius to true
		if (len - 0.1f <= pt[i].radius)
		{
			float ratio = (len - 0.1f) / pt[i].radius;
			if (ratio < smallestRatio) smallestRatio = ratio;
			inRadius = true;
		}
	}
	//set saturation
	if (inRadius)
		sat = 1 - smallestRatio;
	return clamp(sat, 0, 1);
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