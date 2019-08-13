#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D colorTexture[2];

void main() 
{   	
	const float exposure = 1.8;
	const float gamma = 2.2;
    vec3 hdrColor = texture(colorTexture[0], texCoord).rgb; 
	vec3 bloomColor = texture(colorTexture[1], texCoord).rgb;
    hdrColor += bloomColor; // additive blending
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    //mapped = pow(mapped, vec3(1.0 / gamma));
  
    fragColor = vec4(mapped, 1.0);
	
//	const float gamma = 2.2;
//    vec3 hdrColor = texture(screenTexture, texCoord).rgb;
//  
//    // reinhard tone mapping
//    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
//    // gamma correction 
//    //mapped = pow(mapped, vec3(1.0 / gamma));
//
//	fragColor = vec4(mapped, 1.0);
}

