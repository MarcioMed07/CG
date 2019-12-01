#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texC;

out vec2 v_TexC;

uniform mat4 u_ProjectionM;
uniform mat4 u_ViewM;
uniform mat4 u_ModelM;

void main(){
	gl_Position = u_ProjectionM * u_ViewM * u_ModelM *  vec4(position, 1.0f);
	v_TexC = texC;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexC;

uniform sampler2D u_Texture;
uniform sampler2D u_DisplaceMap;
uniform float u_Maximum;
uniform float u_TimeTexture;
uniform float u_TimeDisplace;
uniform float u_TextureAlpha;

void main(){
	
	vec2 displaceMov = vec2(v_TexC.x + u_TimeDisplace, v_TexC.y + u_TimeDisplace);
	vec4 displaceTex = texture2D(u_DisplaceMap, displaceMov);
	float displaceConst = displaceTex.b * u_Maximum;
	vec2 textureMov = vec2(v_TexC.x + displaceConst + u_TimeTexture, v_TexC.y + displaceConst - u_TimeTexture);
	vec4 newTex = texture2D(u_Texture, textureMov);
	newTex.a = u_TextureAlpha;
	gl_FragColor = newTex;
}

