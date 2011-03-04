------------ Vertex Shader for OpenGL 3.0 ------------

DeclareShader('Vertex.GL20', [[
void main()
{
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

]])

DeclareShader('Vertex.GL21', [[
void main()
{
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

]])

DeclareShader('Vertex.GL33', [[
void main()
{
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

]])

------------ Fragment Shader for OpenGL 3.0 ------------

DeclareShader('Fragment.GL20', [[
uniform sampler2D Texture0;

void main()
{
    gl_FragColor = texture2D( Texture0, gl_TexCoord[0].xy)*gl_Color.a;
}

]])

DeclareShader('Fragment.GL21', [[
uniform sampler2D Texture0;

void main()
{
    gl_FragColor = texture2D( Texture0, gl_TexCoord[0].xy)*gl_Color.a;
}

]])

DeclareShader('Fragment.GL33', [[
uniform sampler2D Texture0;

void main()
{
    gl_FragColor = texture2D( Texture0, gl_TexCoord[0].xy)*gl_Color.a;
}

]])
