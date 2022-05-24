#version 330

in vec2 outUV;
in vec3 outColor;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 ambient;

void main()
{
	// Ambient
	vec3 ambientFinal = ambient + vec3(texture(tex, outUV));

	vec3 result = ambientFinal;
	fragColor = vec4(result, 1.0);

	//fragColor = texture(tex, outUV);
}
