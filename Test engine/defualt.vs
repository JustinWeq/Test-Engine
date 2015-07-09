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

struct PixelInput
{
 float4 position: SV_POSITION;
 float2 tex : TEXCOORD0;
 float3 normal :NORMAL;
 float3 viewDirection : TEXCOORD1;
};

PixelInput DefualtVertexShader(VertexInput input)
{
  PixelInput output;
  float4 worldPosition;

  //Change the position vector to be 4 units for proper matrix calculations
  input.position.w = 1.0f;
  ///werwewewrgrgegrreg

  //Calculate the position of the vertex against the world view and projection matricies
  output.position = mul(input.position,worldMatrix);
  output.position = mul(output.position,viewMatrix);
  output.position = mul(output.position,projectionMatrix);

  //Store the texture coordinates for the pixel shader.
  output.tex = input.tex;
  //Calculate the normal vector against the world matrix only
  output.normal = mul(input.normal,(float3x3)worldMatrix);
  //Normalize the normal vector
  output.normal = normalize(output.normal);

  //Calculate the position of the vertex in the world.
  worldPosition = mul(input.position,worldMatrix);

  //Determine the viewing direction based on the position of the camera
  output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

  //Normalize the viewing direction vector
  output.viewDirection = normalize(output.viewDirection);
  
  
  return output;
}