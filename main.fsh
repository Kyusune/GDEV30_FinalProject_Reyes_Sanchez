#version 330

in vec2 outUV;
in vec3 outColor;
in vec3 outNormal;
in vec3 outPosition;

out vec4 fragColor;

// Global
uniform sampler2D tex;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 specular;
uniform vec3 specComp;

// Spotlight Candle
uniform vec3 ambientSpot;
uniform vec3 diffuseSpot;
uniform vec3 lightPosSpot;
uniform vec3 specularSpot;
uniform vec3 specCompSpot;
uniform float constantSpot;
uniform float linearSpot;
uniform float quadraticSpot;



void main()
{
	// Global
	// Diffuse
	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(lightPos - outPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseFinal = diffuse * diff * vec3(texture(tex, outUV));

	// Specular
	vec3 viewDir = normalize(cameraPos - outPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specularFinal = specular * spec * specComp;

	// Ambient
	vec3 ambientFinal = ambient * vec3(texture(tex, outUV));

	// Spotlight
	// Diffuse
	vec3 lightDirSpot = normalize(lightPosSpot - outPosition);
	float diffSpot = max(dot(norm, lightDirSpot), 0.0);
	vec3 diffuseSpotFinal = diffuseSpot * diffSpot * vec3(texture(tex, outUV));

	// Specular
	vec3 reflectDirSpot = reflect(-lightDirSpot, norm);
	float specSpot = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specularSpotFinal = specularSpot * specSpot * specCompSpot;

	// Ambient
	vec3 ambientSpotFinal = ambientSpot * vec3(texture(tex, outUV));

	// Attenuation
	float distance = length(lightPosSpot - outPosition);
	float attenuation = 1.0 / (constantSpot + linearSpot * distance + quadraticSpot * (distance * distance));
	
	diffuseSpotFinal *= attenuation;
	ambientSpotFinal *= attenuation;
	specularSpotFinal *= attenuation;

	// Results
	vec3 result = ambientFinal + diffuseFinal + specularFinal;
	result += ambientSpotFinal + diffuseSpotFinal + specularSpotFinal;
	fragColor = vec4(result, 1.0);

	//fragColor = texture(tex, outUV);
}
