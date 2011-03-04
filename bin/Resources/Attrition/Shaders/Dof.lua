------------ Vertex Shader for OpenGL 3.0 ------------

DeclareShader('Vertex.GL20', [[
attribute vec2 position;

varying vec2 texcoord;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texcoord = position * vec2(0.5) + vec2(0.5);
}


]])

DeclareShader('Vertex.GL21', [[
attribute vec2 position;

varying vec2 texcoord;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texcoord = position * vec2(0.5) + vec2(0.5);
}


]])

DeclareShader('Vertex.GL30', [[
in vec2 position;
out vec2 texcoord;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texcoord = position * vec2(0.5) + vec2(0.5);
}


]])

------------ Fragment Shader for OpenGL 3.0 ------------

DeclareShader('Fragment.GL20', [[

uniform sampler2D Texture;
//uniform sampler2D bgl_DepthTexture;

varying vec2 texcoord;

#define PI    3.14159265

const float blurclamp = 0.04; // max blur amount
const float bias = 0.2; //aperture - bigger values for shallower depth of field

const float treshold = 0.7; // highlight treshold
const float brightness = 2.0; // highlight brightness
const float saturation = 6.0; // highlight saturation
const float contrast = 6.0; // highlight contrast

const int samples = 10; //samples on the first ring
const int rings = 8; //ring count

uniform float focus; //focus value (0.0-1.0)

vec4 sample(vec2 coo,float d) //processing the sample
{
	vec3 col;

	// with chromatic aberration

	col.r = texture2D(Texture, coo - vec2(-0.04,-0.04)*d).r;
	col.g = texture2D(Texture, coo - vec2(0.04,-0.04)*d).g;
	col.b = texture2D(Texture, coo - vec2(0.0,0.04)*d).b;

	// without chromatic aberration

	//col.r = texture2D(Texture, coo).r;
	//col.g = texture2D(Texture, coo).g;
	//col.b = texture2D(Texture, coo).b;

	// processing bokeh highlight treshold, brightness, contrast and saturation

	float AvgLumR = 0.5;
	float AvgLumG = 0.5;
	float AvgLumB = 0.5;

	vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);

	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = col * brightness;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, saturation);
	vec3 conColor = mix(AvgLumin, satColor, contrast);

	float tresh = max(((col.r+col.g+col.b)*0.33)-treshold, 0.0);

	float brightamount = clamp(d * 30, 0.0, 1.0);

	//return vec4(col + (conColor*brightamount),1.0);
	return vec4(col+(conColor*tresh)*brightamount,1.0);
	//return vec4(col,1.0);
}

void main()
{
	//getting blur amount for depth of field

	//vec4 depth1 = texture2D(bgl_DepthTexture,texcoord);
	vec4 depth1 = vec4( 0.0 );
	float factor = (depth1.x - focus);
	vec2 blur = vec2 (clamp(factor * bias, -blurclamp, blurclamp));

	vec4 col;
	float s;

	col += sample(texcoord,blur); //middle sample

	// processing circles of samples

	for (int i = 0 ; i < rings; i += 1)
	{
		for (int j = 0 ; j < (samples*i); j += 1)
		{
			float step = PI*2.0 / (samples*i);
			float x = cos(j*step)*i*0.1;
			float y = sin(j*step)*i*0.1;

			col += sample(texcoord + vec2(blur.x*x, blur.y*y),blur.x);
			s += 1.0;
		}
	}


	col = col/s;

	gl_FragColor.rgb = col;
	gl_FragColor.a = 1.0;
}
]])

DeclareShader('Fragment.GL21', [[

uniform sampler2D Texture;
uniform sampler2D DepthTexture;

varying vec2 texcoord;

#define PI    3.14159265

const float blurclamp = 0.03; // max blur amount
const float bias = 0.3; //aperture - bigger values for shallower depth of field

const float treshold = 0.7; // highlight treshold
const float brightness = 2.0; // highlight brightness
const float saturation = 6.0; // highlight saturation
const float contrast = 6.0; // highlight contrast

const int samples = 10; //samples on the first ring
const int rings = 3; //ring count

uniform float focus; //focus value (0.0-1.0)

vec4 sample(vec2 coo,float d) //processing the sample
{
	vec3 col;

	// with chromatic aberration

	col.r = texture2D(Texture, coo - vec2(-0.04,-0.04)*d).r;
	col.g = texture2D(Texture, coo - vec2(0.04,-0.04)*d).g;
	col.b = texture2D(Texture, coo - vec2(0.0,0.04)*d).b;

	// without chromatic aberration

	//col.r = texture2D(Texture, coo).r;
	//col.g = texture2D(Texture, coo).g;
	//col.b = texture2D(Texture, coo).b;

	// processing bokeh highlight treshold, brightness, contrast and saturation

	float AvgLumR = 0.5;
	float AvgLumG = 0.5;
	float AvgLumB = 0.5;

	vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);

	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = col * brightness;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, saturation);
	vec3 conColor = mix(AvgLumin, satColor, contrast);

	float tresh = max(((col.r+col.g+col.b)*0.33)-treshold, 0.0);

	float brightamount = clamp(d * 30, 0.0, 1.0);

	//return vec4(col + (conColor*brightamount),1.0);
	return vec4(col+(conColor*tresh)*brightamount,1.0);
	//return vec4(col,1.0);
}

void main()
{
	//getting blur amount for depth of field

	vec4 depth1 = texture2D(DepthTexture,texcoord);
	//vec4 depth1 = vec4( 0.0 );
	float factor = (depth1.x - focus);
	vec2 blur = vec2 (clamp(factor * bias, -blurclamp, blurclamp));

	vec4 col;
	float s;

	col += sample(texcoord,blur); //middle sample

	// processing circles of samples

	for (int i = 0 ; i < rings; i += 1)
	{
		for (int j = 0 ; j < (samples*i); j += 1)
		{
			float step = PI*2.0 / (samples*i);
			float x = cos(j*step)*i*0.1;
			float y = sin(j*step)*i*0.1;

			col += sample(texcoord + vec2(blur.x*x, blur.y*y),blur.x);
			s += 1.0;
		}
	}


	col = col/s;

	gl_FragColor.rgb = col;
	gl_FragColor.a = 1.0;
}
]])

DeclareShader('Fragment.GL30', [[

uniform sampler2D Texture;
//uniform sampler2D bgl_DepthTexture;

in vec2 texcoord;
out vec4 FragColor;

#define PI    3.14159265

const float blurclamp = 0.03; // max blur amount
const float bias = 0.1; //aperture - bigger values for shallower depth of field

const float treshold = 0.7; // highlight treshold
const float brightness = 2.0; // highlight brightness
const float saturation = 6.0; // highlight saturation
const float contrast = 6.0; // highlight contrast

const int samples = 10; //samples on the first ring
const int rings = 3; //ring count

uniform float focus; //focus value (0.0-1.0)

vec4 sample(vec2 coo,float d) //processing the sample
{
	vec3 col;

	// with chromatic aberration

	col.r = texture2D(Texture, coo - vec2(-0.04,-0.04)*d).r;
	col.g = texture2D(Texture, coo - vec2(0.04,-0.04)*d).g;
	col.b = texture2D(Texture, coo - vec2(0.0,0.04)*d).b;

	// without chromatic aberration

	//col.r = texture2D(Texture, coo).r;
	//col.g = texture2D(Texture, coo).g;
	//col.b = texture2D(Texture, coo).b;

	// processing bokeh highlight treshold, brightness, contrast and saturation

	float AvgLumR = 0.5;
	float AvgLumG = 0.5;
	float AvgLumB = 0.5;

	vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);

	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = col * brightness;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, saturation);
	vec3 conColor = mix(AvgLumin, satColor, contrast);

	float tresh = max(((col.r+col.g+col.b)*0.33)-treshold, 0.0);

	float brightamount = clamp(d * 30, 0.0, 1.0);

	//return vec4(col + (conColor*brightamount),1.0);
	return vec4(col+(conColor*tresh)*brightamount,1.0);
	//return vec4(col,1.0);
}

void main()
{
	//getting blur amount for depth of field

	//vec4 depth1 = texture2D(bgl_DepthTexture,texcoord);
	vec4 depth1 = vec4( 0.0 );

	float factor = (depth1.x - focus);
	vec2 blur = vec2 (clamp(factor * bias, -blurclamp, blurclamp));

	vec4 col = vec4(0.0);
	float s = 0.0;

	col += sample(texcoord,blur.x); //middle sample

	// processing circles of samples

	for (int i = 0 ; i < rings; i += 1)
	{
		for (int j = 0 ; j < (samples*i); j += 1)
		{
			float step = PI*2.0 / (samples*i);
			float x = cos(j*step)*i*0.1;
			float y = sin(j*step)*i*0.1;

			col += sample(texcoord + vec2(blur.x*x, blur.y*y),blur.x);
			s += 1.0;
		}
	}


	col = col/s;

	//FragColor= vec4(vec3(0.7), 1.0);
	FragColor= vec4(col.rgb, 1.0);
}
]])
