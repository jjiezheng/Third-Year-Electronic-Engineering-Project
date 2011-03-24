

function make_earth()

	local earth = model()
	earth:load( "world.obj", "GroundFromSpace" )
	--earth:load( "world.obj", "SkyFromSpace" )
	earth:texture( "Texture1", "world", 1 ):texture( "Texture0", "worldnight", 0 )
	--earth:load( ".obj", "SkyFromSpace" )
	earth:depth( true ):writeToDepth( true ):ccw( true )--:type( "Lines" )

	earth:uniform( "Model", mat4(1) )
	earth:uniform( "Lighting", mat4(1) )
	earth:uniform( "View", mat4(1) )
	earth:uniform( "Projection", perspective( 45.0, 16.0/10.0, 1, 10000.0 ) )


	local waveLength = vec3( 1/(0.650^4), 1/(0.570^4), 1/( 0.475^4) )
	local light = normalise( vec3(5000,0,0000) )

	local RayleighConst = 0.0025
	local MieConst = 0.0015
	local SunConst = 15

	local RayleighScaleDepth = 0.20
	local MieScaleDepth = 0.1
	local MieAsymmetry = -0.95

	local OuterRadius = 10.25
	local InnerRadius = 10
	local fScale = 1/(OuterRadius - InnerRadius)

	local cameraPos = vec3( 0,0,0 )
	earth:uniform( "v3CameraPos", cameraPos )
	earth:uniform( "v3LightPos", light )
	earth:uniform( "v3InvWavelength", waveLength )

	earth:uniform( "fInnerRadius", InnerRadius )
	earth:uniform( "fInnerRadius2", InnerRadius*InnerRadius )
	earth:uniform( "fOuterRadius", OuterRadius )
	earth:uniform( "fOuterRadius2", OuterRadius*OuterRadius )

	earth:uniform( "fCameraHeight", length(cameraPos) )
	earth:uniform( "fCameraHeight2", length(cameraPos)^2 )

	earth:uniform( "fScale", fScale )
	earth:uniform( "fScaleDepth", RayleighScaleDepth )
	earth:uniform( "fScaleOverScaleDepth", fScale/RayleighScaleDepth )

	earth:uniform( "fKrESun", RayleighConst*SunConst )
	earth:uniform( "fKmESun", MieConst*SunConst )
	earth:uniform( "fKr4PI", RayleighConst*4*math.pi )
	earth:uniform( "fKm4PI", MieConst*4*math.pi )

	earth:uniform( "g", MieAsymmetry )
	earth:uniform( "g2", MieAsymmetry^2 )

	return earth
end
