#version 430

out vec4 outColor;

in vec4 vNormal;
in vec4 vPosition;
in vec2 UV;

in mat4 TBN;

layout(location = 0)uniform mat4 proj;
layout(location = 1)uniform mat4 view;

layout(location = 3)uniform sampler2D diffuseMap;
layout(location = 4)uniform sampler2D specularMap;
layout(location = 5)uniform sampler2D normalMap;

layout(location = 6) uniform int toggle;

in vec4 eyeview;


vec4 cross4(in vec4 a, in vec4 b) { return vec4(cross(a.xyz,b.xyz),0); }


mat4 cotangent_frame(in vec4 N, in vec4 p, in vec2 uv );

uniform vec4 lDir = normalize(vec4(1,0,-1,0));

void main()
{
	vec4 pp = proj * view * vPosition;

	float sP = 8;

	mat4 TBN = cotangent_frame(vNormal, vPosition, UV);
	vec4 N   = TBN * (texture(normalMap, UV) * 2 - 1);


	vec4  R = -reflect(lDir, N);
	vec4  V = normalize(eyeview);

	float spec = dot(R, V);
	
	if(spec > 0)
		spec = pow(spec, sP);
	else spec = 0;

	float lamb = max(0,-dot(N, lDir));

	vec4 diffuse  = lamb * texture( diffuseMap, UV);
	vec4 specular = spec * texture(specularMap, UV);

	outColor = N;
}





mat4 cotangent_frame(in vec4 N, in vec4 p, in vec2 uv )
{
    vec4 dp1  = dFdx( p );
    vec4 dp2  = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    vec4 dp2perp = cross4( dp2, N );
    vec4 dp1perp = cross4( N, dp1 );
 
    vec4 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec4 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat4( T, B, N, vec4(0,0,0,1) );
}