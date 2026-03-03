#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragement
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform samplerCube sceneCubemap;
uniform vec3 cameraPos;
uniform vec3 blackHolePos;
uniform float blackHoleRadius; //schwarzschild radius - r_s
uniform mat4  invView;
uniform mat4 invProjection;

const int   MAX_STEPS = 128;
//const float STEP_SIZE = 0.2;
const float MAX_DIST  = 500.0;

vec3 getRayDir(vec2 uv)
{
    vec4 clipSpace   = vec4(uv * 2.0 - 1.0, -1.0, 1.0);
    vec4 viewSpace   = invProjection * clipSpace;
    viewSpace        = vec4(viewSpace.xy, -1.0, 0.0);
    vec3 worldDir    = normalize((invView * viewSpace).xyz);
    return worldDir;
}

void main()
{
    vec3  rayPos = cameraPos;
    vec3  rayDir = getRayDir(TexCoord);
    float rs     = blackHoleRadius;

    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3  toHole = blackHolePos - rayPos;
        float dist   = length(toHole);

        // absorbed by black hole
        if (dist < rs * 0.5)
        {
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            return;
        }

        if (dist > MAX_DIST) break;

        // adaptive step size - large steps when far, small steps when close
        // this means the ray tracer stays accurate near the black hole
        // but doesn't waste steps in empty space
        float stepSize = max(dist * 0.05, 0.05);

        float bendStrength = (rs * rs) / (dist * dist * dist);
        vec3  bendDir      = normalize(toHole);
        rayDir             = normalize(rayDir + bendDir * bendStrength * stepSize);
        rayPos            += rayDir * stepSize;
    }

    FragColor = texture(sceneCubemap, rayDir);
}