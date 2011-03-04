------------ Vertex Shaders ------------

ogl_21_Vert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;
uniform vec3 DiffuseMaterial;

attribute vec4 Position;
attribute vec3 Normal;
varying vec3 EyespaceNormal;
varying vec3 Diffuse;


void main()
{
	Diffuse = DiffuseMaterial;
	EyespaceNormal = NormalMatrix*Normal;
	gl_Position = Projection*View*Model*Position;

}

]]

oglVert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;
uniform vec3 DiffuseMaterial;

in vec4 Position;
in vec3 Normal;
in vec2 Tex;
out vec2 TexCoord;
out vec3 EyespaceNormal;
out vec3 Diffuse;


void main()
{
	TexCoord = Tex;
	Diffuse = DiffuseMaterial;
	EyespaceNormal = NormalMatrix*Normal;
	gl_Position = Projection*View*Model*Position;
}

]]

DeclareShader('Vertex.GL20', ogl_21_Vert )
DeclareShader('Vertex.GL21', ogl_21_Vert )

DeclareShader('Vertex.GL30', oglVert)
DeclareShader('Vertex.GL31', oglVert )
DeclareShader('Vertex.GL32', oglVert )
DeclareShader('Vertex.GL33', oglVert )

DeclareShader('Vertex.GL40', oglVert )
DeclareShader('Vertex.GL41', oglVert )


------------ Fragment Shaders ------------

ogl_21_Frag = [[

varying vec3 EyespaceNormal;
varying vec3 Diffuse;

uniform vec3 LightPosition;
uniform vec3 AmbientMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;

void main()
{
    vec3 N = normalize(EyespaceNormal);
    vec3 L = normalize(LightPosition);
    vec3 Eye = vec3(0, 0, 1);
    vec3 H = normalize(L + Eye);

    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    vec3 color = AmbientMaterial + df * Diffuse + sf * SpecularMaterial;
    gl_FragColor = vec4(color, 1.0);
}

]]


oglFrag = [[

in vec2 TexCoord;
in vec3 EyespaceNormal;
in vec3 Diffuse;
out vec4 FragColor;

uniform vec3 LightPosition;
uniform vec3 AmbientMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;
uniform sampler2D Texture0;

void main()
{
    vec3 N = normalize(EyespaceNormal);
    vec3 L = normalize(LightPosition);
    vec3 Eye = vec3(0, 0, 1);
    vec3 H = normalize(L + Eye);

    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    vec3 color = AmbientMaterial + df * texture2D( Texture0, TexCoord.st).xyz  + sf * SpecularMaterial;
    FragColor = vec4(color, 1.0);
    //FragColor = texture2D( Texture0, TexCoord.st);
}


]]


DeclareShader('Fragment.GL20', ogl_21_Frag )
DeclareShader('Fragment.GL21', ogl_21_Frag )

DeclareShader('Fragment.GL30', oglFrag )
DeclareShader('Fragment.GL31', oglFrag )
DeclareShader('Fragment.GL32', oglFrag )
DeclareShader('Fragment.GL33', oglFrag )

DeclareShader('Fragment.GL40', oglFrag )
DeclareShader('Fragment.GL41', oglFrag )
