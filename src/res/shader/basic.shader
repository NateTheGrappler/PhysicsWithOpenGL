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

//uniforms that have to do with the lighting
uniform vec3  lightPos;
uniform vec3  lightColor;
uniform vec3  viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;



void main()
{
	vec3 baseColor = useTexture == 1 ? texture(tex, TexCoord).rgb : color;

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
	FragColor   = vec4(result, 1.0);
};
