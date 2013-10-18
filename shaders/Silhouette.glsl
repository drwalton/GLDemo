--UnfScale.Vertex

#version 430

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

out vec3 norm;

uniform mat4 modelToWorld;

uniform float scale;

void main()
{
	norm = mat3(modelToWorld) * vNorm;
	gl_Position = worldToClip * modelToWorld * vec4(scale * vPos, 1.0); //Scale in model space.
}

--NormScale.Vertex

#version 430

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

out vec3 norm;

uniform mat4 modelToWorld;

uniform float scale;

void main()
{
	norm = mat3(modelToWorld) * vNorm;
	gl_Position = worldToClip * modelToWorld * vec4(vPos + vNorm*scale, 1.0) ; //Scale outwards along norm.
}

--ZBias.Vertex

#version 430

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

out vec3 norm;

uniform mat4 modelToWorld;

uniform float bias;

void main()
{
	norm = mat3(modelToWorld) * vNorm;
	gl_Position = worldToClip * modelToWorld * 
		vec4(vPos - vec3(bias * cameraDir), 1.0) ; //Move towards camera by bias.
}

--Fragment

#version 430

in vec3 norm;

out vec4 color;

uniform vec4 outlineColor;

void main()
{
	color = outlineColor;
}
