#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;


uniform mat4 mvp;
uniform mat3 normalM;
uniform mat4 model;
uniform mat4 camera;

out vec2 outUV;
out vec3 outColor;
out vec3 outNormal;
out vec3 outPosition;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 1.0);
	outUV = vertexUV;
	outColor = vertexColor;
	outNormal = normalize(normalM * vertexNormal);
	outPosition = vec3(model * vec4(vertexPosition, 1.0));
}