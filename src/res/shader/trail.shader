#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float alpha;

out vec4 colorVals;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_PointSize = 3.0;
	colorVals = vec4(aColor, alpha);
}

#shader fragement
#version 330 core

in vec4 colorVals;
out vec4 FragColor;

void main()
{
	FragColor = colorVals;
}