#version 400 core

in vec4 clipSpace;

out vec4 outColor;

uniform sampler2D ReflectionTexture;
uniform sampler2D RefractionTexture;

vec2 clipSpaceToTexCoords(vec4 c){
	vec2 ndc = (c.xy / c.w);
	vec2 texCoords = ndc / 2.0 + 0.5;
	return texCoords;
}

void main(void){
	
	vec2 texCoordsReal = clipSpaceToTexCoords(clipSpace);
	
	vec2 RefractionTexCoords = texCoordsReal;
	vec2 ReflectionTexCoords = vec2(texCoordsReal.x, -texCoordsReal.y);
	
	vec3 refractColor = texture(RefractionTexture, RefractionTexCoords).rgb;
	vec3 reflectColor = texture(ReflectionTexture, ReflectionTexCoords).rgb;
	
	vec3 finalColor = mix(reflectColor, refractColor, 0.5);
	//vec3 finalColor = mix(r, vec3(0, 0, 0.8), 0.8);
	//vec3 finalColor = mix(reflectColor, vec3(0, 0, 1), 0.2);
	//vec3 finalColor = refractColor;
	outColor = vec4(finalColor, 1.0);

}