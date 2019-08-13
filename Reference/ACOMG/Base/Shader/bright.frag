#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main() 
{
	fragColor = texture2D(screenTexture, texCoord);

    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (fragColor.r > 0.5 || fragColor.b > 0.5)
        fragColor = vec4(fragColor.rgb, 1.0);
    else
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
}

