//globals
cbuffer Matrix
{
 matrix worldMatrix;
 matrix viewMatrix;
 matrix projectionMatrix;
};

cbuffer Camera
{
 float3 cameraPosition;
 float padding;
};

//typedefs
struct VertexInput
{
 float4 position : POSITION;
 float2 tex :TEXCOORD0;
 float3 normal: NORMAL;
};

struct TextureVertexInput
{
 float4 position : POSITION;
 float2 tex :TEXCOORD0;
};

struct ColorVertexInput
{
 float4 position : POSITION;
 float4 color : COLOR;
};

struct TerrainVertexInput
{
 float4 position : POSITION;
 float2 tex :TEXCOORD0;
 float3 normal : NORMAL;
};


struct PixelInput
{
 float4 position: SV_POSITION;
 float2 tex : TEXCOORD0;
 float3 normal :NORMAL;
 float3 viewDirection : TEXCOORD1;
};

struct TexturePixelInput
{
 float4 position: SV_POSITION;
 float2 tex : TEXCOORD0;
};

struct ColorPixelInput
{
 float4 position : SV_POSITION;
 float4 color : COLOR;
};

struct TerrainPixelInput
{
 float4 position :SV_POSITION;
 float2 tex : TEXCOORD0;
 float3 normal : NORMAL;
};

PixelInput DefualtVertexShader(VertexInput input)
{
     PixelInput output;
    float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	//Calculate the normal vector against the world matrix only
	output.normal = mul(input.normal,(float3x3)worldMatrix);

	//Normalize the normal vector
	output.normal = normalize(output.normal);

	//calculate the position of the vertex in the world.
	worldPosition = mul(input.position,worldMatrix);

	//Determine the viewing direction based on the position of the camera and the position of the vertex in the world
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	//Normalize the viewing direction vector
	output.viewDirection = normalize(output.viewDirection);

    return output;
}

TexturePixelInput textureVertexShader(TextureVertexInput input)
{
    TexturePixelInput output;
    float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	//calculate the position of the vertex in the world.
	worldPosition = mul(input.position,worldMatrix);

    return output;
}

ColorPixelInput colorVertexShader(ColorVertexInput input)
{

 ColorPixelInput output;
 float4 worldPosition;
 
 //Change the position vector to be four units for proper matrix calculations.
 input.position.w = 1.0f;
 
 //Calculate the position of the vertex against the world,view and projection matrices
 output.position  =mul(input.position,worldMatrix);
 output.position  =mul(output.position,viewMatrix);
 output.position = mul(output.position,projectionMatrix);
 
 
 //set the color of the vertex
 output.color = input.color;
 
 return output;
 
}

TerrainPixelInput terrainVertexShader(TerrainVertexInput input)
{
 TerrainPixelInput output;
 //change the position vector to be four units for proper matrix calcualtions
 input.position.w = 1.0f;
 
 //Calculate the position ov the vertex agains the world view and projection matriceis
 output.position = mul(input.position,worldMatrix);
 output.position = mul(output.position,viewMatrix);
 output.position = mul(output.position,projectionMatrix);
 
 //Store the texture coords for the pixel shader
 output.tex = input.tex;
 
 //Calculate the normal vector against the world matrix only
 output.normal = mul(input.normal,(float3x3)worldMatrix);
 
 //normalize the normal vector
 output.normal = normalize(output.normal);
 
 return output;
}






