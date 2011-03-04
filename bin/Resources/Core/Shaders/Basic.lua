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

 //Fragment shader
uniform sampler2D Texture0;
//------------------------
void main()
{
	gl_FragColor = texture2D( Texture0, gl_TexCoord[0].xy);
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

