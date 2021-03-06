#define MAX_TEXTURES 30

//globals
Texture2D shaderTexture[MAX_TEXTURES];;
//Texture2D shaderTextures[MAX_TEXTURES];
//Texture2DArray shaderTexture;
SamplerState SampleType;

cbuffer Light
{
 float4 ambientColor;
 float4 diffuseColor;
 float3 lightDirection;
 float specularPower;
 float4 specularColor;
};

cbuffer PixelColor
{
 float4 pixelColor;
};

struct PixelInput
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
  float3 viewDirection : TEXCOORD1;
};

struct TexturePixelInput
{
 float4 position: SV_POSITION;
 float2 tex : TEXCOORD0;
};

struct ColorPixelInput
{
 float4 position: SV_POSITION;
 float4 color : COLOR;
};

struct TerrainPixelInput
{
 float4 position : SV_POSITION;
 float2 tex :TEXCOORD0;
 float3 normal : NORMAL;
};

struct InstancedPixelInput
{
 float4 position: SV_POSITION;
 float2 tex : TEXCOORD0;
 int textureID : TEXTUREID;
 float4 color :COLOR0;
};


float4 defualtPixelShader(PixelInput input) :SV_TARGET
{
   float4 textureColor;
   float3 lightDir;
   float lightIntensity;
   float4 color;
   float3 reflection;
   float4 specular;

   //Sample the pixel color from the texture using the sampler at this texture coordinate
   textureColor = shaderTexture[0].Sample(SampleType,input.tex);

   //Set the defulat output color to the ambient light value for all pixels.
   color = ambientColor;

   //init the specular color
   specular = float4(0.0f,0.0f,0.0f,0.0f);

   //Invert the light direction for calculations
   lightDir = -lightDirection;

   //Calculate the amount of light on this pixel.
   lightIntensity = saturate(dot(input.normal,lightDir));

   if(lightIntensity > 0.0f)
   {
     //determin the final diffuse color based on the diffuse color and the amount of light
	 color += (diffuseColor * lightIntensity);

	 //Saturate the ambient and diffuse color
	 color = saturate(color);

	 //Calcualte the reflection vector based on the light intensity,normal vector and light direction
	 reflection = normalize(2*lightIntensity*input.normal - lightDir);

	 //Determine the amount of specular light based on the relfection vector,viewing direction and specular power.
	 specular = pow(saturate(dot(reflection,input.viewDirection)),specularPower);
   }

   //Multiply the texture and the final diffuse color to get the final pixel color
   color = color * textureColor;

   //Add the specular componenet last to the output color
   color = saturate(color + specular);
   
   return color;
}

float4 texturePixelShader(TexturePixelInput input) :SV_TARGET
{
   float4 textureColor;

   //Sample the pixel color from the texture using the sampler at this texture coordinate
   textureColor = shaderTexture[0].Sample(SampleType,input.tex);

   
   return textureColor;
}

float4 fontPixelShader(TexturePixelInput input) :SV_TARGET
{
   float4 color;
   //Sample the pixel color from the texture using the sampler at this texture coordinate
   color = shaderTexture[0].Sample(SampleType,input.tex);
   //if the color is black on the texture then treat this pixel as transparent.
   if(color.r == 0.0f)
   {
    color.a = 0.0f;
   }
   //if the color is other then black on the texture then this is a pixel in the font so draw it using the font color.
   else
   {
    color.a = 1.0f;
    color = color*ambientColor;
   }
   

   return color;
}

float4 colorPixelShader(ColorPixelInput input): SV_TARGET
{
 return input.color;
}

float4 terrainPixelShader(TerrainPixelInput input) : SV_TARGET
{
 float4 textureColor;
 float3 lightDir;
 float lightIntensity;
 float4 color;
 
 //Sample the pixel color from the texture using the sampler at this coordinate location
 textureColor = shaderTexture[0].Sample(SampleType,input.tex);
 
 //set the defualt output color to the ambient light value for all pixels
 color = ambientColor;
 
 //invert the light direction for calculations
 lightDir = -lightDirection;
 
 //Calculate the amount of light on this pixel.
 lightIntensity = saturate(dot(input.normal,lightDir));
 
 if(lightIntensity > 0.0)
 {
  //determine the final diffuse color based on the diffuse color and the amount of light intensity
  color += (diffuseColor * lightIntensity);
  }

 
 //saturate the final light color
 color = saturate(color);
 
 //Multiply the texture pixel and the final light color to get the result
 color = color* textureColor;


 return color;
}

float4 instancedPixelShader(InstancedPixelInput input) : SV_TARGET
{
 float4 outputColor;
 float4 textureColor;
 //get the input color
 outputColor = input.color;
 if(input.textureID > -1)
 {
  textureColor = 1;
 //get the texture color
 switch(input.textureID)
 {
  case 0:
  {
   textureColor = shaderTexture[0].Sample(SampleType,input.tex);
   break;
  }
  case 1:
  {
   textureColor = shaderTexture[1].Sample(SampleType,input.tex);
   break;
  }
  case 2:
    {
   textureColor = shaderTexture[2].Sample(SampleType,input.tex);
   break;
  }
  case 3:
    {
   textureColor = shaderTexture[3].Sample(SampleType,input.tex);
   break;
  }
  case 4:
    {
   textureColor = shaderTexture[4].Sample(SampleType,input.tex);
   break;
  }
  case 5:
    {
   textureColor = shaderTexture[5].Sample(SampleType,input.tex);
   break;
  }
  case 6:
    {
   textureColor = shaderTexture[6].Sample(SampleType,input.tex);
   break;
  }
  case 7:
    {
   textureColor = shaderTexture[7].Sample(SampleType,input.tex);
   break;
  }
  case 8:
    {
   textureColor = shaderTexture[8].Sample(SampleType,input.tex);
   break;
  }
  case 9:
    {
   textureColor = shaderTexture[9].Sample(SampleType,input.tex);
   break;
  }
  case 10:
    {
   textureColor = shaderTexture[10].Sample(SampleType,input.tex);
   break;
  }
  case 11:
    {
   textureColor = shaderTexture[11].Sample(SampleType,input.tex);
   break;
  }
  case 12:
    {
   textureColor = shaderTexture[12].Sample(SampleType,input.tex);
   break;
  }
  case 13:
    {
   textureColor = shaderTexture[13].Sample(SampleType,input.tex);
   break;
  }
  case 14:
    {
   textureColor = shaderTexture[14].Sample(SampleType,input.tex);
   break;
  }
  case 15:
    {
   textureColor = shaderTexture[15].Sample(SampleType,input.tex);
   break;
  }
  case 16:
    {
   textureColor = shaderTexture[16].Sample(SampleType,input.tex);
   break;
  }
  case 17:
    {
   textureColor = shaderTexture[17].Sample(SampleType,input.tex);
   break;
  }
  case 18:
    {
   textureColor = shaderTexture[18].Sample(SampleType,input.tex);
   break;
  }
  case 19:
    {
   textureColor = shaderTexture[19].Sample(SampleType,input.tex);
   break;
  }
  case 20:
    {
   textureColor = shaderTexture[20].Sample(SampleType,input.tex);
   break;
  }
  case 21:
    {
   textureColor = shaderTexture[21].Sample(SampleType,input.tex);
   break;
  }
  case 22:
      {
   textureColor = shaderTexture[22].Sample(SampleType,input.tex);
   break;
  }
  
  case 23:
      {
   textureColor = shaderTexture[23].Sample(SampleType,input.tex);
   break;
  }
  case 24:
      {
   textureColor = shaderTexture[24].Sample(SampleType,input.tex);
   break;
  }
  case 25:
      {
   textureColor = shaderTexture[25].Sample(SampleType,input.tex);
   break;
  }
  case 26:
      {
   textureColor = shaderTexture[26].Sample(SampleType,input.tex);
   break;
  }
  case 27:
      {
   textureColor = shaderTexture[27].Sample(SampleType,input.tex);
   break;
  }
  case 28:
      {
   textureColor = shaderTexture[28].Sample(SampleType,input.tex);
   break;
  }
  case 29:
  {
   textureColor = shaderTexture[29].Sample(SampleType,input.tex);
   break;
  }
 }
 
 //multiply the two colors together
  outputColor = textureColor*outputColor;
 }
 //outputColor.r = 1.0f;
// outputColor.g = 1.0f;
 //outputColor.b = 1.0f;
 //outputColor.a = 1.0f;
 if(outputColor.r + outputColor.g+outputColor.b  == 0.0f)
 {
 outputColor.a = 0.0f;
 }
 //return the final output color
 return outputColor;
}

float4 fontPixelShader(InstancedPixelInput input): SV_TARGET
{
float4 outputColor;
float4 textureColor;
//get the input color
outputColor = input.color;
if (input.textureID > -1)
{
	textureColor = 1;
	//get the texture color
	switch (input.textureID)
	{
	case 0:
	{
		textureColor = shaderTexture[0].Sample(SampleType, input.tex);
		break;
	}
	case 1:
	{
		textureColor = shaderTexture[1].Sample(SampleType, input.tex);
		break;
	}
	case 2:
	{
		textureColor = shaderTexture[2].Sample(SampleType, input.tex);
		break;
	}
	case 3:
	{
		textureColor = shaderTexture[3].Sample(SampleType, input.tex);
		break;
	}
	case 4:
	{
		textureColor = shaderTexture[4].Sample(SampleType, input.tex);
		break;
	}
	case 5:
	{
		textureColor = shaderTexture[5].Sample(SampleType, input.tex);
		break;
	}
	case 6:
	{
		textureColor = shaderTexture[6].Sample(SampleType, input.tex);
		break;
	}
	case 7:
	{
		textureColor = shaderTexture[7].Sample(SampleType, input.tex);
		break;
	}
	case 8:
	{
		textureColor = shaderTexture[8].Sample(SampleType, input.tex);
		break;
	}
	case 9:
	{
		textureColor = shaderTexture[9].Sample(SampleType, input.tex);
		break;
	}
	case 10:
	{
		textureColor = shaderTexture[10].Sample(SampleType, input.tex);
		break;
	}
	case 11:
	{
		textureColor = shaderTexture[11].Sample(SampleType, input.tex);
		break;
	}
	case 12:
	{
		textureColor = shaderTexture[12].Sample(SampleType, input.tex);
		break;
	}
	case 13:
	{
		textureColor = shaderTexture[13].Sample(SampleType, input.tex);
		break;
	}
	case 14:
	{
		textureColor = shaderTexture[14].Sample(SampleType, input.tex);
		break;
	}
	case 15:
	{
		textureColor = shaderTexture[15].Sample(SampleType, input.tex);
		break;
	}
	case 16:
	{
		textureColor = shaderTexture[16].Sample(SampleType, input.tex);
		break;
	}
	case 17:
	{
		textureColor = shaderTexture[17].Sample(SampleType, input.tex);
		break;
	}
	case 18:
	{
		textureColor = shaderTexture[18].Sample(SampleType, input.tex);
		break;
	}
	case 19:
	{
		textureColor = shaderTexture[19].Sample(SampleType, input.tex);
		break;
	}
	case 20:
	{
		textureColor = shaderTexture[20].Sample(SampleType, input.tex);
		break;
	}
	case 21:
	{
		textureColor = shaderTexture[21].Sample(SampleType, input.tex);
		break;
	}
	case 22:
	{
		textureColor = shaderTexture[22].Sample(SampleType, input.tex);
		break;
	}

	case 23:
	{
		textureColor = shaderTexture[23].Sample(SampleType, input.tex);
		break;
	}
	case 24:
	{
		textureColor = shaderTexture[24].Sample(SampleType, input.tex);
		break;
	}
	case 25:
	{
		textureColor = shaderTexture[25].Sample(SampleType, input.tex);
		break;
	}
	case 26:
	{
		textureColor = shaderTexture[26].Sample(SampleType, input.tex);
		break;
	}
	case 27:
	{
		textureColor = shaderTexture[27].Sample(SampleType, input.tex);
		break;
	}
	case 28:
	{
		textureColor = shaderTexture[28].Sample(SampleType, input.tex);
		break;
	}
	case 29:
	{
		textureColor = shaderTexture[29].Sample(SampleType, input.tex);
		break;
	}
	}

	//multiply the two colors together
	outputColor = textureColor*outputColor;
}
//outputColor.r = 1.0f;
// outputColor.g = 1.0f;
//outputColor.b = 1.0f;
//outputColor.a = 1.0f;
if (textureColor.r == 0.0f)
{
	outputColor.a = 0.0f;
}
//return the final output color
return outputColor;
}