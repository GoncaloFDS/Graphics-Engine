#version 400 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;

out vec4 outColor;
uniform float moveFactor;
uniform float near;
uniform float far;

uniform sampler2D ReflectionTexture;
uniform sampler2D RefractionTexture;
uniform sampler2D dudvMap; 
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float waveStrenght;
uniform float reflectiveness;
uniform float shineDamper;
uniform vec3 lightColor;
uniform float waterClampValue;

vec2 clipSpaceToTexCoords(vec4 c){
	return (c.xy / c.w) / 2.0 + 0.5;
}

float LightAttenuation(float a, float b, float distance){
	return 1/(1 + a*distance + (b*distance*distance));
}


void main(void){
	
	vec2 texCoordsReal = clipSpaceToTexCoords(clipSpace);
	vec2 RefractionTexCoords = texCoordsReal;
	vec2 ReflectionTexCoords = vec2(texCoordsReal.x, -texCoordsReal.y);

	float depth = texture(depthMap, RefractionTexCoords).r; // float between 0 and 1 (not linear)
	// nerdy stuff to get the distance between the camera and the ground
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));   
	depth = gl_FragCoord.z;
	// nerdy stuff to get the distance between the camera and the water plane
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	float waterDepth = floorDistance - waterDistance;

	vec2 distortedCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
	distortedCoords = textureCoords + vec2(distortedCoords.x, distortedCoords.y + moveFactor);
	vec2 distortion = (texture(dudvMap, distortedCoords).rg * 2.0 - 1.0) * waveStrenght;
	
	RefractionTexCoords += distortion;
 	RefractionTexCoords = clamp(RefractionTexCoords, 0.001, 0.999);

	ReflectionTexCoords += distortion;
 	ReflectionTexCoords.x = clamp(ReflectionTexCoords.x, 0.001, 0.999);
 	ReflectionTexCoords.y = clamp(ReflectionTexCoords.y, -0.999, -0.001);

	vec4 refractColor = texture(RefractionTexture, RefractionTexCoords);
	vec4 reflectColor = texture(ReflectionTexture, ReflectionTexCoords);

	vec3 view = normalize(toCameraVector);
	float frenelFactor = dot(view, vec3(0.0, 1.0, 0.0));
	frenelFactor = pow(frenelFactor, reflectiveness);

	vec4 normalMapColor = texture(normalMap, distortedCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, view) , 0);
	float att = LightAttenuation(0.02, 0.0, length(fromLightVector));
	specular = pow(specular, shineDamper);
	float alpha = clamp(waterDepth/waterClampValue, 0.0, 1.0);
	vec3 specularHighlights = lightColor * specular * reflectiveness * att * alpha;
	
	outColor = mix(reflectColor, refractColor, frenelFactor);
	outColor = mix(outColor, vec4(0, 0.2, 0.4, 1.0), 0.4) + vec4(specularHighlights, 0);
	outColor.a = alpha;
	//outColor = vec4(waterDepth);
	//outColor = vec4(fromLightVector, 1);
	//outColor = texture(depthMap, texCoordsReal);
}