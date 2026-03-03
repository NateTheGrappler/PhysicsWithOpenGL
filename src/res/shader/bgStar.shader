#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec3 starPos;
uniform float starSize;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp    = vec3(view[0][1], view[1][1], view[2][1]);

	vec3 worldPos = starPos 
		+ cameraRight * aPos.x * starSize 
		+ cameraUp * aPos.y * starSize;

	TexCoord = aTexCoord;
	gl_Position = projection * view * vec4(worldPos, 1.0);
}

#shader fragement
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 glowColor;

void main()
{
    //distance from center of quad (0.5, 0.5)
    vec2  center = TexCoord - vec2(0.5);
    float dist   = length(center) * 2.0; // 0 at center, 1 at edge

    //soft glow falloff - bright center fading to transparent edge
    float glow   = exp(-dist * dist * 4.0); // gaussian falloff
    float alpha  = glow;

    //bright white core with colored halo
    vec3 coreColor = mix(glowColor, vec3(1.0, 1.0, 1.0), pow(1.0 - dist, 3.0));

    FragColor = vec4(coreColor * glow, alpha);
}