

function make_earth(_scale)
	local scale = _scale or 1
	local earth = model()
	--earth:load( "world.obj", "GroundFromSpace" )
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

	local OuterRadius = 10.25*scale
	local InnerRadius = 10*scale
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


function gen_lookup_catmull(_positions, resolution)

	local samples = {}
	local pos, old_pos = _positions[2], _positions[2]
	local arc_length = 0

	table.insert(samples, {p=pos,l=length(pos-old_pos), w=vec3(0), u=vec3(0) } )
	for seg=2, #_positions-2 do
		
		for i=1,resolution do
			local j = i/resolution

			pos = catmullRom( _positions[seg-1],
							  _positions[seg],
					  		  _positions[seg+1],
					  		  _positions[seg+2],
					  		  j )

			local w = catmull_w(
				_positions[seg-1],
				_positions[seg],
				_positions[seg+1],
				_positions[seg+2],
				j )

			local u = catmull_u(
				_positions[seg-1],
				_positions[seg],
				_positions[seg+1],
				_positions[seg+2],
				j )

			arc_length = arc_length + length(pos-old_pos)
			table.insert(samples, {p=pos,l=arc_length, w=w, u=u } )
			old_pos = pos
		end
	end
	return samples
end


function move_closure(points)

	local samples = gen_lookup_catmull(points,20)
	local dist = 0
	local low,high = 1,2

	return function(_deltaTime)

		dist = dist + _deltaTime*0.02
		
		while (samples[high].l < dist )  do
			if high < #samples-2 then
				low,high = low+1, low+2
			else 
				Log("Gameover\n\n\n\n\n\n\n")
				return {p=samples[high].p,
						w=samples[high].w,
						u=samples[high].u}, true
			 end
		end

		local p_dist = samples[high].l - samples[low].l
		local cur_dist = dist - samples[low].l
		local ratio = cur_dist/p_dist

		return { p=mix(samples[low].p,samples[high].p,ratio),
				 w=mix(samples[low].w,samples[high].w,ratio),
				 u=mix(samples[low].u,samples[high].u,ratio) }, false
	end
end














function catmull_dydx1(p0, p1, p2, p3, t)
	local a = p2-p0
	local b = ( (p0*2) - (p1*5) + (p2*4) - p3 )*2*t
	local c = ( (p1*3) - p0 - (p2*3) + p3)*3*t*t
	
	return ((a+b+c)*0.5)
end


function catmull_dydx2(p0, p1, p2, p3, t)
	local b = ( (p0*2) - (p1*5) + (p2*4) - p3 )*2
	local c = ( (p1*3) - p0 - (p2*3) + p3)*6*t
	
	return ((b+c)*0.5)
end

function catmull_w(p0, p1, p2, p3, t)
	return  normalise( catmull_dydx1(p0, p1, p2, p3, t) )
end

function catmull_u(p0, p1, p2, p3, t)
	return normalise( cross(
		catmull_dydx1(p0, p1, p2, p3, t),
		catmull_dydx2(p0, p1, p2, p3, t)) )
end



function make_world_fancy_camera(_positons)

	local positions = _positons

	local ori = {}
	for i = 1, #positions do
		ori[i] = { pos = positions[i] }
	end

	ori.current = { rot = math.uQuat(), pos = vec3(0) }

	local rot_timer, unit_time = 0, 5000
	local cur_pos, new_position = 2,2
	local change_in_position = false

	local movement = move_closure(positions)
	return 
	{
		logic = function(_deltaTime)
			local current, finished = movement(_deltaTime)
			return current.p, lookAt(vec3(0)-current.w, vec3(0), vec3(0,1,0) ), finished
		end
	}
end