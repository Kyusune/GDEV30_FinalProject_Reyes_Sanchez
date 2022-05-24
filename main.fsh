#version 330

in vec2 outUV;
in vec3 outColor;
in vec3 outNormal;
in vec3 outPosition;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 lightPos;

void main()
{
	// Diffuse
	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(lightPos - outPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseFinal = diffuse * diff * vec3(texture(tex, outUV));

	// Ambient
	vec3 ambientFinal = ambient * vec3(texture(tex, outUV));

	vec3 result = ambientFinal + diffuseFinal;
	fragColor = vec4(result, 1.0);

	//fragColor = texture(tex, outUV);
}
