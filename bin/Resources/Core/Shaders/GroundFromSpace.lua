------------ Vertex Shaders ------------

oglVert = [[
//
// Atmospheric scattering vertex shader
//
// Author: Sean O'Neil - Edited by Daniel Hartnett
//
// Copyright (c) 2004 Sean O'Neil
//

uniform vec3 v3CameraPos;		// The camera's current position
uniform vec3 v3LightPos;		// The direction vector to the light source
uniform vec3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
uniform float fCameraHeight;	// The camera's current height
uniform float fCameraHeight2;	// fCameraHeight^2
uniform float fOuterRadius;		// The outer (atmosphere) radius
uniform float fOuterRadius2;	// fOuterRadius^2
uniform float fInnerRadius;		// The inner (planetary) radius
uniform float fInnerRadius2;	// fInnerRadius^2
uniform float fKrESun;			// Kr * ESun
uniform float fKmESun;			// Km * ESun
uniform float fKr4PI;			// Kr * 4 * PI
uniform float fKm4PI;			// Km * 4 * PI
uniform float fScale;			// 1 / (fOuterRadius - fInnerRadius)
uniform float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
uniform float fScaleOverScaleDepth;	// fScale / fScaleDepth

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Lighting;

in vec4 Position;
in vec2 UV_0;
out vec2 TexCoord;
out vec4 FirstColor;
out vec4 SecondColor;

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void)
{
	int nSamples = 4;
	float fSamples = 4.0;

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	vec3 camPos = v3CameraPos;
	vec3 v3Pos = Position.xyz;
	vec3 v3light = v3LightPos;
	v3Pos = (Model*vec4(Position.xyz, 1.0 )).xyz;

	vec3 v3Ray = v3Pos - camPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float B = 2.0 * dot(camPos, v3Ray);
	float C = fCameraHeight2 - fOuterRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	float fNear = 0.5 * (-B - sqrt(fDet));

	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = camPos + v3Ray * fNear;
	fFar -= fNear;

	float fDepth = exp((fInnerRadius - fOuterRadius) / fScaleDepth);
	float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);
	float fLightAngle = dot(v3light, v3Pos) / length(v3Pos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
	vec3 v3Attenuate;
	for(int i=0; i<nSamples;i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	FirstColor.rgb = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);
	SecondColor.rgb = v3Attenuate;

	gl_Position = Projection*View*Model*Position;
	TexCoord = UV_0;
}



]]

DeclareShader('Vertex.GL20', oglVert )
DeclareShader('Vertex.GL21', oglVert )

DeclareShader('Vertex.GL30', oglVert)
DeclareShader('Vertex.GL31', oglVert )
DeclareShader('Vertex.GL32', oglVert )
DeclareShader('Vertex.GL33', oglVert )

DeclareShader('Vertex.GL40', oglVert )
DeclareShader('Vertex.GL41', oglVert )


------------ Fragment Shaders ------------


oglFrag = [[

//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil - Edited by Daniel Hartnett
//
// Copyright (c) 2004 Sean O'Neil
//

uniform sampler2D Texture0;
uniform sampler2D Texture1;

in vec4 FirstColor;
in vec4 SecondColor;
in vec2 TexCoord;
out vec4 FragColor;

void main (void)
{
	vec4 mapNight = texture2D( Texture0, TexCoord );
	vec4 map = texture2D( Texture1, TexCoord );
	FragColor = FirstColor +  0.30*SecondColor*map + 0.50*(1.0-SecondColor.r)*mapNight*mapNight;
}



]]


DeclareShader('Fragment.GL20', oglFrag )
DeclareShader('Fragment.GL21', oglFrag )

DeclareShader('Fragment.GL30', oglFrag )
DeclareShader('Fragment.GL31', oglFrag )
DeclareShader('Fragment.GL32', oglFrag )
DeclareShader('Fragment.GL33', oglFrag )

DeclareShader('Fragment.GL40', oglFrag )
DeclareShader('Fragment.GL41', oglFrag )
