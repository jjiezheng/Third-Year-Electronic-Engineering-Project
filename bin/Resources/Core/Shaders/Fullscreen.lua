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

uniform sampler2D Texture;
uniform vec2  radialPosition;

uniform float timer;
uniform float sampleStrength;
uniform float Contrast;
uniform float width;
uniform float height;

varying vec2 texcoord;

void main(void)
{
	gl_FragColor = texture2D(Texture, texcoord);
}

]]




Vignette = [[

uniform sampler2D Texture;
uniform vec2  radialPosition;

uniform float timer;
uniform float sampleStrength;
uniform float Contrast;
uniform float width;
uniform float height;


varying vec2 texcoord;



const float noise_amount = -0.03;
const float sampleDist = 0.5;


#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
vec3 BlendOverlay(vec3 base, vec3 blend)
{
  return vec3(BlendOverlayf(base.r, blend.r), BlendOverlayf(base.g, blend.g), BlendOverlayf(base.b, blend.b) );
}



// create a black and white oval about the center of our image for our vignette
vec4 generateVignette(vec2 normalizedTexcoord)
{
	normalizedTexcoord = 2.0 * normalizedTexcoord - 1.0;
	float r = length(normalizedTexcoord)*0.8;
	return 1.0 - vec4(smoothstep(0.5,1.0,r)) + 0.5;
}


//Generate Noise
vec4 generateNoise()
{
	float noiseR =  (fract(sin(dot(texcoord ,vec2(12.9898,78.233)+timer)) * 43758.5453));
	float noiseG =  (fract(sin(dot(texcoord ,vec2(12.9898,78.233)+timer*2.0)) * 43758.5453));
	float noiseB =  (fract(sin(dot(texcoord ,vec2(12.9898,78.233)+timer*3.0)) * 43758.5453));
	return vec4(noiseR,noiseG,noiseB,1.0)*noise_amount;
}




void main(void)
{
	vec4 original = texture2D(Texture, texcoord);
	vec4 vignetteResult = generateVignette(texcoord);
	vec4 noiseResult = generateNoise();

	gl_FragColor = vec4(BlendOverlay(original.rgb, vignetteResult.rgb*0.35 ),original.a) + noiseResult;
	//gl_FragColor = original;
}

]]




oldStyleFragFirst = [[


uniform sampler2D Texture;
uniform vec2  radialPosition;

uniform float timer;
uniform float sampleStrength;
uniform float Contrast;
uniform float width;
uniform float height;


varying vec2 texcoord;



const float noise_amount = -0.03;
const float sampleDist = 0.5;

#define KERNEL_SIZE 9

// Gaussian kernel
// 1 2 1
// 2 4 2
// 1 2 1
const float kernel[KERNEL_SIZE] = { 1.0/16.0, 2.0/16.0, 1.0/16.0,
				2.0/16.0, 4.0/16.0, 2.0/16.0,
				1.0/16.0, 2.0/16.0, 1.0/16.0 };

const float step_w = 1.0/width;
const float step_h = 1.0/height;

const vec2 offset[KERNEL_SIZE] = { vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h),
				vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0),
				vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h) };



vec4 generateGaussianBlur(vec2 _texcoord)
{
  int i = 0;
  vec4 sum = vec4(0.0);

  for( i=0; i<KERNEL_SIZE; i++ )
  {
    vec4 tmp = texture2D(Texture, _texcoord + offset[i]);
    sum += tmp * kernel[i];
  }
  return sum;
}


#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
vec3 BlendOverlay(vec3 base, vec3 blend)
{
  return vec3(BlendOverlayf(base.r, blend.r), BlendOverlayf(base.g, blend.g), BlendOverlayf(base.b, blend.b) );
}



// create a black and white oval about the center of our image for our vignette
vec4 generateVignette(vec2 normalizedTexcoord)
{
	normalizedTexcoord = 2.0 * normalizedTexcoord - 1.0;
	float r = length(normalizedTexcoord)*0.8;
	return 1.0 - vec4(smoothstep(0.5,1.0,r)) + 0.5;
}

/*
** Contrast, saturation, brightness
** Code of this function is from TGM's shader pack
** http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=21057
*/

// For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
vec3 ContrastSaturationBrightness(vec3 color, float con, float sat, float brt)
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



vec4 generateRadialBlur(vec2 normalizedTexcoord)
{
  float samples[20] = float[](0.05, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1);

  vec2 dir = radialPosition - normalizedTexcoord;
  float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
  dir = dir/dist;

  vec4 color = texture2D(Texture, normalizedTexcoord);
  vec4 sum = color;

  for (int i = 0; i < 20; i++)
  {
    sum += texture2D( Texture, normalizedTexcoord + dir * samples[i] * sampleDist );
  }

  sum *= 1.0/21.0;

  float t = dist * sampleStrength;
  t = clamp( t, 0.0, 1.0);


  vec4 blur = mix( color, sum, t );
  vec4 satBlur = vec4( ContrastSaturationBrightness(blur.rgb, Contrast, Contrast, Contrast), blur.a );



  return satBlur;
  //return mix( color, sum, t );
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
	inputDistort.r = texture2D(Texture,rCoords).r;
	inputDistort.g = texture2D(Texture,gCoords).g;
	inputDistort.b = texture2D(Texture,bCoords).b;
	inputDistort.a = texture2D(Texture,Texcoord).a;

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




void main(void)
{
	//vec4 original = texture2D(Texture, texcoord);
	//vec4 vignetteResult = generateVignette(texcoord);
	vec4 chromaticResult = generateChromatic(texcoord);
	chromaticResult.rgb = ContrastSaturationBrightness( chromaticResult.rgb, Contrast, 1.0, 1.0 );
	//vec4 radialBlurResult = generateRadialBlur(texcoord);
	//vec4 gausBlur = generateGaussianBlur(texcoord);
	vec4 noiseResult = generateNoise();


  //gl_FragColor = vec4( BlendOverlay( original.rgb, gausBlur.rgb ), original.a );
  //gl_FragColor = gausBlur;
  gl_FragColor = vec4( chromaticResult.rgb,chromaticResult.a ) + noiseResult;
  //gl_FragColor = vec4( radialBlurResult.rgb,radialBlurResult.a );
  //gl_FragColor = vec4(BlendOverlay(original.rgb, vignetteResult.rgb*0.35 ),original.a) + noiseResult;
  //gl_FragColor = original;
}

]]





oldStyleFragSecond = [[


uniform sampler2D Texture;
uniform vec2  radialPosition;

uniform float timer;
uniform float sampleStrength;
uniform float Contrast;
uniform float width;
uniform float height;


varying vec2 texcoord;



const float noise_amount = -0.03;
const float sampleDist = 0.5;

#define KERNEL_SIZE 9

// Gaussian kernel
// 1 2 1
// 2 4 2
// 1 2 1
const float kernel[KERNEL_SIZE] = { 1.0/16.0, 2.0/16.0, 1.0/16.0,
				2.0/16.0, 4.0/16.0, 2.0/16.0,
				1.0/16.0, 2.0/16.0, 1.0/16.0 };

const float step_w = 1.0/width;
const float step_h = 1.0/height;

const vec2 offset[KERNEL_SIZE] = { vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h),
				vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0),
				vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h) };



vec4 generateGaussianBlur(vec2 _texcoord)
{
  int i = 0;
  vec4 sum = vec4(0.0);

  for( i=0; i<KERNEL_SIZE; i++ )
  {
    vec4 tmp = texture2D(Texture, _texcoord + offset[i]);
    sum += tmp * kernel[i];
  }
  return sum;
}


#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
vec3 BlendOverlay(vec3 base, vec3 blend)
{
  return vec3(BlendOverlayf(base.r, blend.r), BlendOverlayf(base.g, blend.g), BlendOverlayf(base.b, blend.b) );
}



// create a black and white oval about the center of our image for our vignette
vec4 generateVignette(vec2 normalizedTexcoord)
{
	normalizedTexcoord = 2.0 * normalizedTexcoord - 1.0;
	float r = length(normalizedTexcoord)*0.8;
	return 1.0 - vec4(smoothstep(0.5,1.0,r)) + 0.5;
}

/*
** Contrast, saturation, brightness
** Code of this function is from TGM's shader pack
** http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=21057
*/

// For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
vec3 ContrastSaturationBrightness(vec3 color, float con, float sat, float brt)
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



vec4 generateRadialBlur(vec2 normalizedTexcoord)
{
  float samples[20] = float[](0.05, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1);

  vec2 dir = radialPosition - normalizedTexcoord;
  float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
  dir = dir/dist;

  vec4 color = texture2D(Texture, normalizedTexcoord);
  vec4 sum = color;

  for (int i = 0; i < 20; i++)
  {
    sum += texture2D( Texture, normalizedTexcoord + dir * samples[i] * sampleDist );
  }

  sum *= 1.0/21.0;

  float t = dist * sampleStrength;
  t = clamp( t, 0.0, 1.0);


  vec4 blur = mix( color, sum, t );
  vec4 satBlur = vec4( ContrastSaturationBrightness(blur.rgb, Contrast, Contrast, Contrast), blur.a );



  return satBlur;
  //return mix( color, sum, t );
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
	inputDistort.r = texture2D(Texture,rCoords).r;
	inputDistort.g = texture2D(Texture,gCoords).g;
	inputDistort.b = texture2D(Texture,bCoords).b;
	inputDistort.a = texture2D(Texture,Texcoord).a;

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




void main(void)
{
	//vec4 original = texture2D(Texture, texcoord);
	//vec4 vignetteResult = generateVignette(texcoord);
	//vec4 chromaticResult = generateChromatic(texcoord);
	vec4 radialBlurResult = generateRadialBlur(texcoord);
	//vec4 gausBlur = generateGaussianBlur(texcoord);
	vec4 noiseResult = generateNoise();


  //gl_FragColor = vec4( BlendOverlay( original.rgb, gausBlur.rgb ), original.a );
  //gl_FragColor = gausBlur;
  //gl_FragColor = vec4( chromaticResult.rgb,chromaticResult.a ) + noiseResult;
  gl_FragColor = vec4( radialBlurResult.rgb,radialBlurResult.a );
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

DeclareShader('Fragment.GL20.Vignette', Vignette )
DeclareShader('Fragment.GL21.Vignette', Vignette )

DeclareShader('Fragment.GL30.Vignette', Vignette )
DeclareShader('Fragment.GL31.Vignette', Vignette )
DeclareShader('Fragment.GL32.Vignette', Vignette )
DeclareShader('Fragment.GL33.Vignette', Vignette )

DeclareShader('Fragment.GL40.Vignette', Vignette )
DeclareShader('Fragment.GL41.Vignette', Vignette )



DeclareShader('Fragment.GL20.First', oldStyleFragFirst )
DeclareShader('Fragment.GL21.First', oldStyleFragFirst )

DeclareShader('Fragment.GL30.First', oldStyleFragFirst )
DeclareShader('Fragment.GL31.First', oldStyleFragFirst )
DeclareShader('Fragment.GL32.First', oldStyleFragFirst )
DeclareShader('Fragment.GL33.First', oldStyleFragFirst )

DeclareShader('Fragment.GL40.First', oldStyleFragFirst )
DeclareShader('Fragment.GL41.First', oldStyleFragFirst )



DeclareShader('Fragment.GL20.Second', oldStyleFragSecond )
DeclareShader('Fragment.GL21.Second', oldStyleFragSecond )

DeclareShader('Fragment.GL30.Second', oldStyleFragSecond )
DeclareShader('Fragment.GL31.Second', oldStyleFragSecond )
DeclareShader('Fragment.GL32.Second', oldStyleFragSecond )
DeclareShader('Fragment.GL33.Second', oldStyleFragSecond )

DeclareShader('Fragment.GL40.Second', oldStyleFragSecond )
DeclareShader('Fragment.GL41.Second', oldStyleFragSecond )

