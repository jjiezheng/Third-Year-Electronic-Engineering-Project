------------ Vertex Shaders ------------

oldStyleVert = [[

attribute vec2 position;

varying vec2 texcoord;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texcoord = position * vec2(0.5) + vec2(0.5);
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

uniform sampler2D textures[2];
//uniform sampler1D Curve;
uniform float timer;

varying vec2 texcoord;

uniform vec2 radialPosition;

const float noise_amount = -0.03;
const float sampleDist = 1.0;
const float sampleStrength = 1.8;




// create a black and white oval about the center of our image for our vignette
vec4 generateVignette(vec2 normalizedTexcoord)
{
	normalizedTexcoord = 2.0 * normalizedTexcoord - 1.0;
	float r = length(normalizedTexcoord)*0.8;
	return 1.0 - vec4(smoothstep(0.5,1.0,r)) + 0.5;
}




vec4 generateRadialBlur(vec2 normalizedTexcoord)
{
  float samples[20] = float[](0.05, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1);

  vec2 dir = radialPosition - normalizedTexcoord;
  float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
  dir = dir/dist;

  vec4 color = texture2D(textures[0], normalizedTexcoord);
  vec4 sum = color;

  for (int i = 0; i < 20; i++)
  {
    sum += texture2D( textures[0], normalizedTexcoord + dir * samples[i] * sampleDist );
  }

  sum *= 1.0/21.0;

  float t = dist * sampleStrength;
  t = clamp( t, 0.0, 1.0);


  return mix( color, sum, t );
}




//Generate Noise
vec4 generateChromatic(vec2 Texcoord)
{
  //Generate chromatic dispersion
  float dispersion = 0.02;
  float f = 0.99;
  float scale = 1.0;

	//index of refraction of each color channel, causing chromatic dispersion
	vec3 eta = vec3(1.0+dispersion*0.9, 1.0+dispersion*0.6, 1.0+dispersion*0.3);

	// get the right pixel for the current position
	vec2 rCoords = (f*eta.r)*scale*(texcoord-0.5)+0.5;
	vec2 gCoords = (f*eta.g)*scale*(texcoord-0.5)+0.5;
	vec2 bCoords = (f*eta.b)*scale*(texcoord-0.5)+0.5;

	vec4 inputDistort;
	inputDistort.r = texture2D(textures[0],rCoords).r;
	inputDistort.g = texture2D(textures[0],gCoords).g;
	inputDistort.b = texture2D(textures[0],bCoords).b;
	inputDistort.a = texture2D(textures[0],Texcoord).a;

	return inputDistort;
}



//Generate Noise
vec4 generateNoise()
{
	float noiseR =  (fract(sin(dot(texcoord ,vec2(12.9898,78.233)+timer)) * 43758.5453));
	float noiseG =  (fract(sin(dot(texcoord ,vec2(12.9898,78.233)+timer*2.0)) * 43758.5453));
	float noiseB =  (fract(sin(dot(texcoord ,vec2(12.9898,78.233)+timer*3.0)) * 43758.5453));
	return vec4(noiseR,noiseG,noiseB,1.0)*noise_amount;
}



#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
vec3 BlendOverlay(vec3 base, vec3 blend)
{
  return vec3(BlendOverlayf(base.r, blend.r), BlendOverlayf(base.g, blend.g), BlendOverlayf(base.b, blend.b) );
}




void main(void)
{
  vec4 original = texture2D(textures[0], texcoord);
	vec4 vignetteResult = generateVignette(texcoord);
  vec4 chromaticResult = generateChromatic(texcoord);
  vec4 radialBlurResult = generateRadialBlur(texcoord);
  vec4 noiseResult = generateNoise();



  //gl_FragColor = vec4( chromaticResult.rgb,chromaticResult.a ) + noiseResult;
  gl_FragColor = vec4( radialBlurResult.rgb,radialBlurResult.a ) + noiseResult;
  //gl_FragColor = vec4(BlendOverlay(original.rgb, vignetteResult.rgb*0.35 ),original.a);
  //gl_FragColor = vec4(BlendOverlay(original.rgb, vignetteResult.rgb*0.35 ),original.a) + noiseResult;
  //gl_FragColor = original;
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

