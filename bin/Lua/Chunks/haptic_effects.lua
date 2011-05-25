if haptics.included == nill then
	local start_effect = haptics.start_effect

	haptics.start_effect = function( effect )
		return start_effect(
			effect.name or "constant",
			effect.pos  or vec3(0),
			effect.dir  or vec3(0),
			effect.gain or 0,
			effect.mag  or 0 )
	end
	haptics.included = true
end

function normalSpace( _matrix )
	return inverseTranspose( mat3(_matrix) )
end


function  make_wind()
	local wind = {}
	wind.reload = true
	wind.name = "constant"
	
	wind.gain = 0.1
	wind.mag = 0.05
	wind.dir = vec3( 0, 0, 0 )
	wind.handle = haptics.start_effect{wind}

	return function()
		local x = math.random( -1, 1 )
		local y = math.random( -1, 1 )
		local z = math.random( -1, 1 )
		wind.dir = vec3( x, y, z )


		haptics.stop_effect( wind.handle )
		wind.handle = haptics.start_effect(wind)
	end
end
