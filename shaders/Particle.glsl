--Particle.Vertex

#version 330

in vec2 startPos;
in uint startTime;
in float tex;

uniform mat4 modelToWorld;

uniform uint lifetime;
uniform uint time;
uniform vec4 coeffts;
uniform float flameHeight;

uniform float windWidth;
uniform float windHeight;

out VertexData {
		float decay;
		vec2 tex;
	} VertexOut;

float spline(float x, vec4 c);

void main()
{
	float decay = float(time) / float(lifetime);
	float radius = spline(decay, coeffts);
	VertexOut.decay = decay;
	VertexOut.tex.x = tex * (1.0 - windWidth);
	VertexOut.tex.y = height * (1.0 - windHeight);
	gl_Position = modelToWorld * vec4(
		startPos.x * radius, 
		decay * flameHeight, 
		startPos.y * radius, 
		1.0);
}

float spline(float x, vec4 c)
{
	return (((c.x * x + c.y) * x + c.z) * x + c.w) * x + 1.0;
}

--Particle.Geometry

#version 330

layout(points) in;

layout(triangle_strip, max_vertices = 4) out;

in VertexData {
	float decay;
	float tex;
} VertexIn[];

uniform float bbWidth;
uniform float bbHeight;

uniform float windWidth;
uniform float windHeight;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

out float decay;
out vec2 tex;
out vec2 bbPos;

void main()
{
	decay = VertexIn[0].decay;
	vec3 pointPos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(-vec3(cameraDir));

	// Find vectors in plane of billboard.
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 across = normalize(cross(up, toCamera));
	up = normalize(cross(toCamera, across));

	vec3 corner;
	// Bottom left vertex
	corner = pointPos - (0.5*bbWidth*across) - (0.5*bbHeight*up);
	gl_Position = worldToCamera * vec4(corner, 1.0);
	bbPos = vec2(0, 0);
	tex = VertexIn[0].tex;
	EmitVertex();

	// Top left vertex
	corner = pointPos - (0.5*bbWidth*across) + (0.5*bbHeight*up);
	gl_Position = worldToCamera * vec4(corner, 1.0);
	bbPos = vec2(0, 1);
	tex = VertexIn[0].tex + vec2(0.0, windHeight);
	EmitVertex();

	// Bottom right vertex
	corner = pointPos + (0.5*bbWidth*across) - (0.5*bbHeight*up);
	gl_Position = worldToCamera * vec4(corner, 1.0);
	bbPos = vec2(1, 0);
	tex = VertexIn[0].tex + vec2(windWidth, 0.0);
	EmitVertex();

	// Top right vertex
	corner = pointPos + (0.5*bbWidth*across) + (0.5*bbHeight*up);
	gl_Position = worldToCamera * vec4(corner, 1.0);
	bbPos = vec2(1, 1);
	tex = VertexIn[0].tex + vec2(windWidth, windHeight);
	EmitVertex();

	EndPrimitive();
}

--Particle.Fragment

#version 330

in float decay;
in vec2 tex;
in vec2 bbPos;

uniform sampler2D bbTex;
uniform sampler1D decayTex;

out vec4 color;

float alphaFunc(float x);

void main()
{
	//Fade out edges of billboard radially.
	vec2 fromCenter = (bbPos - vec2(0.5, 0.5)) * 2.0;
	float centerDistSq = clamp(dot(fromCenter, fromCenter), 0.0, 1.0);
	float fade = (1 - centerDistSq);

	//Fade alpha according to decay value.
	float alpha = alphaFunc(decay) * fade;

	//Apply billboard texture's alpha.
	alpha *= texture(bbTex, tex).a;

	//Apply decay texture's color.
	color = vec4(vec3(texture(decayTex, decay)), alpha);
}

/* A quartic function which rapidly increases,
 * then decreases slowly back to 0 slowly over the interval [0,1].
 */
float alphaFunc(float x)
{
	return (((-2*x + 8)*x - 12)*x + 6)*x;
}