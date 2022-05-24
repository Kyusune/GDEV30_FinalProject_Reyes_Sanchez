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
uniform vec3 cameraPos;
uniform vec3 specular;

void main()
{

	// Diffuse
	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(lightPos - outPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseFinal = diffuse * diff * vec3(texture(tex, outUV));

	//Specular
	vec3 viewDir = normalize(cameraPos - outPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specularFinal = specular * spec * vec3(texture(tex, outUV));

	// Ambient
	vec3 ambientFinal = ambient * vec3(texture(tex, outUV));

	vec3 result = ambientFinal + diffuseFinal + specularFinal;
	fragColor = vec4(result, 1.0);

	//fragColor = texture(tex, outUV);
}
