#type vertex
#version 450 core
		
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_Tiling;
layout(location = 5) in int a_EntityID;
		
//layout(std140, binding = 0) uniform Camera{
//mat4 u_ViewProjection;
//};

struct VertexOutput{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;

};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityID;

uniform mat4 u_ViewProjection;
//uniform mat4 u_ModelTransform;

//out vec4 v_Color;	
//out vec2 v_TexCoord;
//out float v_TexIndex;
//out float v_Tiling;
//out flat int v_EntityID;

void main(){
	//v_Color = a_Color;
	//v_TexCoord = a_TexCoord;
	//v_TexIndex = a_TexIndex;
	//v_Tiling = a_Tiling;

	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TexIndex = a_TexIndex;
	Output.TilingFactor = a_Tiling;

	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Position , 1.0);
}

#type fragment
#version 450 core
		
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

//in vec4 v_Color;
//in vec2 v_TexCoord;
//in float v_TexIndex;
//in float v_Tiling;

struct VertexOutput{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];
		
void main(){

	color = texture(u_Textures[int(Input.TexIndex)], Input.TexCoord * Input.TilingFactor ) * Input.Color;
	color2 = v_EntityID;
	//color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_Tiling ) * v_Color;
	//color2 = v_EntityID;
}