

function make_earth(_scale)
	local scale = _scale or 1
	local earth = model()
	--earth:load( "world.obj", "GroundFromSpace" )
	earth:load( "sky.obj", "GroundFromSpace" )
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

	local OuterRadius = 10.50*scale
	local InnerRadius = 10.25*scale
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


function make_world_fancy_camera()

	local quats = 
	{ 
		rotateQuat( math.uQuat(), 0, vec3(0,1,0) ),
		rotateQuat( math.uQuat(), 0, vec3(0,1,0) ), 
		rotateQuat( rotateQuat( math.uQuat(), -30, vec3(0,0,1) ), -30, vec3(0,1,0) ), 
		rotateQuat( rotateQuat( math.uQuat(), -100, vec3(0,0,1) ),  -50, vec3(0,1,0) ), 
		rotateQuat( rotateQuat( math.uQuat(), -200, vec3(0,0,1)  ), -70, vec3(0,1,0) ), 
		rotateQuat( rotateQuat( math.uQuat(), -30, vec3(0,0,1) ), -90, vec3(0,1,0) ), 
		rotateQuat( rotateQuat( math.uQuat(), -180, vec3(0,0,1) ), -110, vec3(0,1,0) )
	}

	local positions = 
	{	
		vec3(0,0,1000),
		vec3(0,0,800),
		vec3(300,0,300),
		vec3(400,0,100),
		vec3(400,0,0),
		vec3(400,0,-100),
		vec3(400,0,-100)
	}

	local ori = {}
	for i = 1, #positions do
		ori[i] = { rot = quats[i], pos = positions[i] }
	end

	ori.current = { rot = math.uQuat(), pos = vec3(0) }


	local rot_timer, unit_time = 0, 50000
	local cur_pos, new_position = 2,2
	local change_in_position = false
	return 
	{
		logic = function(_deltaTime)

			rot_timer = rot_timer + _deltaTime
			if rot_timer > unit_time and cur_pos < (#ori - 2) then
				rot_timer = rot_timer - unit_time
				cur_pos = cur_pos + 1
			end
			
			local t = smoothstep( 0, unit_time, rot_timer )

			local inter1 = intermediate( ori[cur_pos-1].rot,
										 ori[cur_pos].rot,
										 ori[cur_pos+1].rot )

			local inter2 = intermediate( ori[cur_pos].rot,
										 ori[cur_pos+1].rot,
										 ori[cur_pos+2].rot )


			ori.current.rot = squad( normalise(ori[cur_pos].rot),
									 normalise(ori[cur_pos+1].rot),
									 normalise(inter1),
									 normalise(inter2),
									 smoothstep( 0, unit_time, rot_timer ) )


			ori.current.pos = catmullRom( ori[cur_pos-1].pos,
										  ori[cur_pos].pos,
										  ori[cur_pos+1].pos,
										  ori[cur_pos+2].pos,
										  smoothstep( 0, unit_time, rot_timer ) )


			return ori.current.pos, scale( mat4Cast( ori.current.rot ), vec3(1) )
		end
	}
end