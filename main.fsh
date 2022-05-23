#version 330

in vec2 outUV;
in vec3 outColor;
in vec3	outNormal;
in vec3 outPosition;
in mat4 outModel;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camera;
uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform float diffMat;
uniform float specularIntensity;
uniform float specMat;
uniform float shine;

void main()
{
	// Diffuse
	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(lightPos - outPosition);
	float diff = max(dot(norm, lightDir), diffMat);
	vec3 diffuse = diffuseIntensity * diff * vec3(texture(tex, outUV));

	// Ambient
	vec3 ambient = ambientIntensity * vec3(texture(tex, outUV));
	
	//Specular
	vec3 view = normalize(camera - outPosition);
	vec3 reflect = reflect(-lightDir, norm);
	float spec = pow(max(dot(view, reflect), specMat), shine);
	vec3 specular = specularIntensity * spec * vec3(texture(tex, outUV));

	// Summation
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}
