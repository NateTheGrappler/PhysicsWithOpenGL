#shader vertex
#version 330 core

//this specifies the location of the data in the array buffer that is sent
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 normal;
out vec3 FragPos;

//projection stuff for the camera
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//set both the position for the vertex given what is in the array, and also set up vertex color to be pased in
	FragPos = vec3(model * vec4(aPos.xyz, 1.0));
	normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * aPos; 
};



#shader fragement
#version 330 core

in vec2 TexCoord;
in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;	

uniform sampler2D tex;
uniform int useTexture;
uniform vec3 color;
uniform vec2 uvScale;

//uniforms that have to do with the lighting
uniform vec3  lightPos;
uniform vec3  lightColor;
uniform vec3  viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

//torus uniforms
uniform int   isDisk;
uniform vec3  diskCenter;
uniform float diskInnerRadius;
uniform float diskOuterRadius;

void main()
{
	// disk gradient - early return so normal lighting doesnt interfere
	if (isDisk == 1)
	{
		float dist = length(FragPos.xz - diskCenter.xz);
		float t    = clamp((dist - diskInnerRadius) / (diskOuterRadius - diskInnerRadius), 0.0, 1.0);

		vec3 innerColor  = vec3(1.0,  0.95, 0.8);
		vec3 middleColor = vec3(1.0,  0.5,  0.05);
		vec3 outerColor  = vec3(0.4,  0.02, 0.0);

		vec3 diskColor;
		if (t < 0.5)
			diskColor = mix(innerColor, middleColor, t * 2.0);
		else
			diskColor = mix(middleColor, outerColor, (t - 0.5) * 2.0);

		// fade out at edges and add some noise-like variation based on angle
		float angle      = atan(FragPos.z - diskCenter.z, FragPos.x - diskCenter.x);
		float swirl      = sin(angle * 8.0 + diskInnerRadius) * 0.1 + 0.9;
		float alpha      = (1.0 - t * t) * 0.95 * swirl;

		FragColor = vec4(diskColor * swirl, alpha);
		return;
	}

	vec3 baseColor = useTexture == 1 ? texture(tex, TexCoord * uvScale).rgb : color;
	float alpha = useTexture == 1 ? texture(tex, TexCoord * uvScale).a : 1.0;

	//ambient base light level
	vec3 ambient = ambientStrength * lightColor;

	//diffuse, the angle between normal and light direction
	vec3 norm		   = normalize(normal);
	vec3 lightDir	   = normalize(lightPos - FragPos);
	float diff         = max(dot(norm, lightDir), 0.0);
	vec3 diffuse       = diffuseStrength * diff * lightColor;

	//specular, the reflection
	vec3 viewDir       = normalize(viewPos - FragPos);
	vec3 reflectionDir = reflect(-lightDir, norm);
	float spec         = pow(max(dot(viewDir, reflectionDir), 0.0), 32.0);
	vec3 specular      = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * baseColor;
	FragColor   = vec4(result, alpha);
};
