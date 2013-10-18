--Vertex

#version 330

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

out vec3 norm;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

uniform mat4 modelToWorld;

void main()
{
	norm = mat3(modelToWorld) * vNorm;
	gl_Position = worldToClip * modelToWorld * vec4(vPos, 1.0);
}

--Fragment

#version 330

in vec3 norm;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

out vec4 color;

uniform vec4 modelColor;
uniform int numLevels;

float step(float val, int levels)
{
	int level = int(val * levels);
	return float(level) / levels;
}

void main()
{
	float camDotN = clamp(dot(-vec3(cameraDir), norm), 0, 1);
	float brightness = step(camDotN, numLevels);
	color = (0.5 + brightness/2.0) * modelColor; 
}
