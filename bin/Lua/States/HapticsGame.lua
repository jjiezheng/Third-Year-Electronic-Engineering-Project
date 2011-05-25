dofile( include( "math" ) )
dofile( include( "ExamineCamera" ) )
dofile( include( "haptic_effects" ) )
dofile( include( "make_earth" ) )

Projection = perspective( 45.0, 16.0/10.0, 1, 10000.0 )
View = mat4(1.0)

quats = 
{ 
	rotateQuat( math.uQuat(), 0, vec3(0,1,0) ),
	rotateQuat( math.uQuat(), 0, vec3(0,1,0) ), 
	rotateQuat( rotateQuat( math.uQuat(), -30, vec3(0,0,1) ), -30, vec3(0,1,0) ), 
	rotateQuat( rotateQuat( math.uQuat(), -100, vec3(0,0,1) ),  -50, vec3(0,1,0) ), 
	rotateQuat( rotateQuat( math.uQuat(), -200, vec3(0,0,1)  ), -70, vec3(0,1,0) ), 
	rotateQuat( rotateQuat( math.uQuat(), -30, vec3(0,0,1) ), -90, vec3(0,1,0) ), 
	rotateQuat( rotateQuat( math.uQuat(), -180, vec3(0,0,1) ), -110, vec3(0,1,0) )
}

quatslol = 
{ 
	rotateQuat( math.uQuat(), 0, vec3(0,1,0) ),
	rotateQuat( math.uQuat(), 0, vec3(0,1,0) ), 
	math.uQuat(),
	math.uQuat(),
	math.uQuat(),
	math.uQuat(),
	math.uQuat()
}

positions = 
{	
	vec3(0,0,1000),
	vec3(0,0,800),
	vec3(300,0,300),
	vec3(400,0,100),
	vec3(400,0,0),
	vec3(400,0,-100),
	vec3(400,0,-100)
}

positionslol = 
{	
	vec3(0,0,0),
	vec3(0,0,0),
	vec3(300,0,0),
	vec3(400,0,0),
	vec3(400,0,0),
	vec3(400,0,0),
	vec3(400,0,0)
}


function gen_lookup_catmull(_positions, resolution)

	local samples = {}
	local pos, old_pos = _positions[2], _positions[2]
	local arc_length = 0

	table.insert(samples, {p=pos,l=length(pos-old_pos) } )
	for seg=2, #_positions-2 do
		
		for i=1,resolution do
			local j = i/resolution

			pos = catmullRom( _positions[seg-1],
							  _positions[seg],
					  		  _positions[seg+1],
					  		  _positions[seg+2],
					  		  j )

			arc_length = arc_length + length(pos-old_pos)
			table.insert(samples, {p=pos,l=arc_length } )
			old_pos = pos
		end
	end
	return samples
end


function enemy_move_closure(points)

	local samples = gen_lookup_catmull(points,20)
	local dist = 0
	local low,high = 1,1

	return function(_deltaTime)

		dist = dist + _deltaTime*0.01
		
		while (samples[high].l < dist )  do
			if high < #samples then
				low,high = low+1
				high = low+1
			else return samples[high].p end
		end

		local p_dist = samples[high].l - samples[low].l
		local cur_dist = dist - samples[low].l
		local ratio = cur_dist/p_dist

		return mix(samples[low].p,samples[high].p,ratio) 
	end
end



function normalSpace( _matrix )
	return inverseTranspose( mat3(_matrix) )
end



function make_haptics_player()

	local model = mesh()
	model:load( "ship2.3ds", "CelShaded" )
	model:uniform( "Model", mat4(1) );
	model:uniform( "Projection", Projection )
	model:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	model:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	model:uniform( "Shininess", 100 )

	model:uniform( "NormalMatrix", normalSpace(View) )
	model:uniform( "View", View )
	model:uniform( "LightPosition", vec3( -10, 10, 10 ) )
	model:depth( true )
	model:writeToDepth( true )

	return
	{
		logic = function(_delta,_trans, _camera)
			local _model = rotate( rotate( scale(_trans, vec3(2)), 90, vec3(1,0,0)), 180,vec3(0,1,0))
			model:uniform( "Model", _model )

			model:uniform( "View", _camera )
			model:uniform( "NormalMatrix", normalSpace(_camera*_model) )
		end,
		render = function()
			model:render()
		end,
		reload = function()
			model:reload()
		end
	}
end








function astroid_postition(_pos, _i, _blend)
	return catmullRom( _pos[_i-1],
					   _pos[_i],
					   _pos[_i+1],
					   _pos[_i+2],
					   _blend ) - vec3(0,0,0)
end




function make_astroids(_positions, _quats, _number)
	local astroids = {}
	local positions = positions
	local number_of_points = #positions - 3
	local roids_between_points = _number/number_of_points
	local blend = 0
	local current = 2

	for i=1,_number do
		astroids[i] = model()
		astroids[i]:load( "gaspra.3ds", "TexturedVbo" )
		astroids[i]:texture( "Texture0", "gaspra", 0 )
		astroids[i]:depth( true )
		astroids[i]:writeToDepth( true )
		

		blend = (i%roids_between_points)/roids_between_points
		if i%roids_between_points == 0 and i~=_number then 
			current = current+1
		end
		Log( "current:" .. tostring(current))
		Log("")
		local X = math.random( -10, 10 )
		local Y = math.random( -10, 10 )
		local Z = math.random( 0, 1000 )
		local _X = math.random( -1, 1 )
		local _Y = math.random( -1, 1 )
		local _Z = math.random( -1, -1 )
		local _amount = math.random()
		astroids[i].pos = vec3( X,Y,Z )
		astroids[i].dir = vec3( 0,0,0.01 )
		astroids[i].rot = vec3( _X, _Y, _Z )
		astroids[i].rotAmount = _amount/10

		
		
		astroids[i].pos = astroid_postition(positions, current, blend)
		--astroids[i].dir = catmull_direction(positions, current, blend)
		
		astroids[i].model_matrix = rotate( scale( translate( mat4(1), astroids[i].pos ), vec3(0.5) ), 90, vec3(1,0,0))
		
		astroids[i]:uniform( "Model",astroids[i].model_matrix )
		astroids[i]:uniform( "View", mat4(1) )
		astroids[i]:uniform( "Projection", Projection )
	end

	return 
	{
		logic = function(_delta,_camera)

			for i, v in ipairs(astroids) do
				v:uniform( "View", _camera )
			end
			
			--[[for i, v in ipairs(self.astroids) do
				v.pos = v.pos + v.dir*_deltaTime

				v:uniform( "View", translate( self.camera.view, v.pos ) )
				v:uniform( "Model",
					rotate( v.model_matrix, v.rotAmount*self.time_passed, v.rot) )

				local distance = position - v.pos

				local x = distance:x()
				local y = distance:y()
				local z = distance:z()
				
				if x < 0 then x = -x end
				if y < 0 then y = -y end
				if z < 0 then z = -z end

				if x < 3 and
				   y < 3 and
				   z < 3 then 
					self.got_hit = true 
				end
			end


			if self.got_hit then
				self.got_hit = false
				self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0, 0 ) )
			else
				self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
			end--]]
		end,
		render = function()
			for i, v in ipairs(astroids) do
				v:render()
			end
		end,
		reload = function()
			for i, v in ipairs(astroids) do
				v:reload()
			end
		end	
	}
end































HapticsGame = {}
function HapticsGame.Start(self)

	sys.title( "Haptics" )
	haptics.workspace( perspective( 65.0, 16.0/10.0, 1, 10.0 ), mat4(1) )

	self.camera = ExamineCamera()
	self.wind = make_wind()
	self.player = make_haptics_player()
	self.astroids = make_astroids(positions, quats, 100)


	self.world_camera = make_world_fancy_camera(positions,quats)
	self.world = make_earth(10)
	self.angle = 0
	self.speed_up = false
	self.got_hit = false
	self.time_passed = 0
end


function HapticsGame.HandleEvents( self, _events )

	return false
end

















function HapticsGame.Logic( self, _deltaTime )

	self.time_passed = self.time_passed + _deltaTime
	self.camera:logic( _deltaTime )
	local position = haptics.proxy_position()
	local trans = haptics.proxy_transform()

	self.wind()
	local _camera = self.earth_logic( self, _deltaTime )

	self.player.logic( _deltaTime, trans, self.camera.view )
	self.astroids.logic( _deltaTime, _camera )
end

function HapticsGame.earth_logic( self, _deltaTime )

	local pos, rot = self.world_camera.logic(_deltaTime)
	self.angle = self.angle + 0.0005*_deltaTime
	local cameraPos = pos  - vec3(0)*mat3(rot)
	local _camera = (mat4(1)*rot*translate( mat4(1), vec3(0)-pos) )

	self.world:uniform( "Model", 
		rotate( rotate( 
			scale(mat4(1.0),vec3(10)), 23.44, vec3(0,0,-1) ), 
			self.angle,vec3(0,1,0) ) )

	self.world:uniform( "View", _camera )

	self.world:uniform( "v3CameraPos", cameraPos )
	self.world:uniform( "fCameraHeight", length(cameraPos) )
	self.world:uniform( "fCameraHeight2", length(cameraPos)^2 )

	return _camera
end


























function HapticsGame.Render(self)
	self.world:render()
	self.player.render()
	self.astroids.render()
end

function HapticsGame.Reload(self)
	self.world:reload()
	self.player.reload()
	self.astroids.reload()	
end