#shader vertex
#version 330 core

//this specifies the location of the data in the array buffer that is sent
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//set both the position for the vertex given what is in the array, and also set up vertex color to be pased in
   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
   TexCoord = aTexCoord;
};



#shader fragement
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;		  

uniform sampler2D tex;
uniform int useTexture;
uniform vec3 color;

void main()
{
	if (useTexture == 1)
		FragColor = texture(tex, TexCoord);
	else
		FragColor = vec4(color.xyz, 1.0);
};
