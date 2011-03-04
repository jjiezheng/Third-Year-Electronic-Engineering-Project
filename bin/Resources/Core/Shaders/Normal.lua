------------ Vertex Shaders ------------

oldStyleVert = [[

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

]]

DeclareShader('Vertex.GL20', oldStyleVert )
DeclareShader('Vertex.GL21', oldStyleVert )

DeclareShader('Vertex.GL30', oldStyleVert )
DeclareShader('Vertex.GL31', oldStyleVert )
DeclareShader('Vertex.GL32', oldStyleVert )
DeclareShader('Vertex.GL33', oldStyleVert )

DeclareShader('Vertex.GL40', oldStyleVert )
DeclareShader('Vertex.GL41', oldStyleVert )


------------ Fragment Shaders ------------

oldStyleFrag = [[

 uniform sampler2D Texture0;

uniform float Brightness = 1.0;
uniform float Contrast = 1.0;
uniform float Saturation = 1.0;
uniform float Glow = 1.0;

 // For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)
{
	// Increase or decrease theese values to adjust r, g and b color channels seperately
	const float AvgLumR = 0.5;
	const float AvgLumG = 0.5;
	const float AvgLumB = 0.5;

	const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);

	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = color * brt;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, sat);
	vec3 conColor = mix(AvgLumin, satColor, con);
	return conColor;
}

//------------------------
void main()
{
	vec4 texel = texture2D( Texture0, gl_TexCoord[0].xy);
	//vec3 bsc = ContrastSaturationBrightness( texel.rgb, Brightness, Saturation, Contrast );

	//texel = vec4( bsc, texel.a );
	gl_FragColor = texel
}

]]


DeclareShader('Fragment.GL20', oldStyleFrag )
DeclareShader('Fragment.GL21', oldStyleFrag )

DeclareShader('Fragment.GL30', oldStyleFrag )
DeclareShader('Fragment.GL31', oldStyleFrag )
DeclareShader('Fragment.GL32', oldStyleFrag )
DeclareShader('Fragment.GL33', oldStyleFrag )

DeclareShader('Fragment.GL40', oldStyleFrag )
DeclareShader('Fragment.GL41', oldStyleFrag )
