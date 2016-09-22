#version 430

layout(location = 0) uniform mat4 view;
layout(location = 1) uniform mat4 proj;

layout(location = 2) uniform sampler2D albedoMap;
layout(location = 3) uniform sampler2D normalMap;
layout(location = 4) uniform sampler2D specularMap;
layout(location = 5) uniform sampler2D positionMap;
layout(location = 6) uniform sampler2D depthMap;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec4 outSpecular;

in vec2 vUV;

layout(location = 7) uniform vec4 lDir;// = normalize(vec4(1,-1,-1,0));
layout(location = 8) uniform vec4 lCol;// = vec4(1,1,1,1);


void main()
{
	if(gl_FragCoord.z > texture(depthMap,vUV).r)
		discard;

	vec3 L = normalize((view * lDir).xyz);
	vec3 N = normalize(texture(normalMap, vUV).xyz);

	vec3 R = reflect(L, N);
	vec3 E = normalize(view[3].xyz + texture(positionMap,vUV).xyz);
	float sP = 2;

	float lamb = max(0,-dot(L, N));
	float spec = max(0,-dot(E, R));
	if(spec > 0)
			spec = pow(spec, sP);

	outAlbedo   = texture(albedoMap,   vUV) * lamb * lCol;
	outSpecular = texture(specularMap, vUV) * spec * lCol;
	outColor = outAlbedo + outSpecular;
}