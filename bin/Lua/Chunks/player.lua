

--Closure for Setting how the ship behaves when it moves
function turningClosure()

	local animationTime = 125
	local moving = false
	local time = 0
	local current_direction = 1
	local direction = 1
	local starting_angle = 1
	local turning_angle = 20
	local angle = 1

	--Behaviour which banks the ship in the direction it is turning
	return function( _player_matrix, _dir, _deltaTime )

		if _dir:x() > 0.01 or _dir:x() < -0.01 then

			if _dir:x() > 0 then direction = -1 else direction = 1 end

			if not moving or current_direction ~= direction then
				moving = true
				current_direction = direction
				time = 0
			end

			time = time + _deltaTime
			if time > animationTime then time = animationTime end
			angle = mix( starting_angle, turning_angle*direction, smoothstep( 0, animationTime, time ) )

			return rotate( _player_matrix, angle, vec3(0,1,0) )
		else

			moving = false
			time = time - _deltaTime
			if time < 0 then time = 0 end
			angle = mix( 0, 15*direction, smoothstep( 0, animationTime, time ) )
			starting_angle = angle

			return  rotate( _player_matrix, angle, vec3(0,1,0) )
		end
	end
end



















--Closure for Ship Controls
function controlsClosure( _controls )

	local dir = vec3(0)
	local fire = 0
	local controls = {}

	if type(_controls) ~= "table" then
		Log( "Not a fucking table")
	else controls = _controls end


	if controls.up == nil then	controls.up = key.w	end
	if controls.down == nil then controls.down = key.s	end
	if controls.left == nil then controls.left = key.a	end
	if controls.right == nil then controls.right = key.d end
	if controls.fire1 == nil then controls.fire1 = key.k end
	if controls.fire2 == nil then controls.fire2 = key.l end


	--Default Controls
	return function( _events )

		if events.isKeyDown( _events, controls.up ) and events.isKeyDown( _events, controls.down ) then
			dir:set_y(0)
		elseif events.isKeyDown( _events, controls.up ) then
			dir:set_y(1)
		elseif events.isKeyDown( _events, controls.down ) then
			dir:set_y(-1)
		else
			dir:set_y(0)
		end


		if events.isKeyDown( _events, controls.left ) and events.isKeyDown( _events, controls.right ) then
			dir:set_x(0)
		elseif events.isKeyDown( _events, controls.left ) then
			dir:set_x(-1)
		elseif events.isKeyDown( _events, controls.right ) then
			dir:set_x(1)
		else
			dir:set_x(0)
		end

		if events.isKeyDown( _events, key.z ) then
			fire = 1
		else fire = 0 end

		if dir:x() ~= 0 and dir:y() ~= 0 then
			dir = dir*0.71
		end

		return dir, fire
	end
end








--Closure for Ship Movement
function movementClosure()

	local move = vec3(0)

	--Default Movement
	return function( _info, _deltaTime )

		move = _info.dir*0.11*_deltaTime
		_info.pos = _info.pos + move

		if _info.pos:x() + _info.hitbox:x() > 80 then _info.pos:set_x(80 - _info.hitbox:x())   end
		if _info.pos:x() - _info.hitbox:x() <  -80 then _info.pos:set_x(_info.hitbox:x() - 80)  end
		if _info.pos:y() + _info.hitbox:y() >  100 then _info.pos:set_y(100 - _info.hitbox:y())  end
		if _info.pos:y() - _info.hitbox:y() < -100 then _info.pos:set_y(_info.hitbox:y()-100) end

		return _info.pos
	end
end












function load_player_ship( _name, _view, _modelMatrix )

	local ship = model()
	--ship:load( "HeadlessGiant", "CelShaded" )
	--ship:load( "CameraRollAnim.3ds", "CelShaded" )
	ship:load( _name, "CelShaded" )
	ship.model = _modelMatrix


	ship:uniform( "Model", _modelMatrix )
	ship:uniform( "View", _view )
	ship:uniform( "Projection", Projection )
	ship:uniform( "NormalMatrix", normalSpace(_modelMatrix) )

	ship:uniform( "LightPosition", vec3(0,0,500) )
	ship:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	ship:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	ship:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	ship:uniform( "Shininess", 100 )
	ship:depth( true )
	ship:writeToDepth( true )

	return ship
end
















function make_player( _view, _fire )

	if _fire == nil then
		error("no fire function included")
	end

	sys.load_settings( "player_controls" )

	if type(player_controls) ~= "table" then
		Log("Not a table here either!")
	end
	local player = {}
	local firing_function = _fire

	player.vel = vec3(0)
	player.dir = vec3(0)
	player.pos = vec3(0)
	player.fire = 0
	player.modelMatrix = scale(
		translate( rotate(
			rotate(
				mat4(1.0),
				180,
				vec3( 1,0,0) ),
			180,
			vec3( 0,1,0) ),
			vec3(0,0,-0) ),
		vec3(5) )

	player.hitbox = collision.obb(5,5)
	player.hitbox:z(-241.0)
	player.render_hitbox = collision.visualHitbox( player.hitbox )
	player.render_hitbox:uniform( "Colour", vec3(0.16863,1,0.1647) )
	player.render_hitbox:uniform( "ProjectionView", Projection*mat4(1.0) )


	player.ship = load_player_ship( "ship.3ds",_view, player.modelMatrix )

	local move = movementClosure()
	local ship_turning = turningClosure()
	local ship_controls = controlsClosure(player_controls)


	--Event Handler
	function player.handleEvents( self, _events )
		self.dir, self.fire = ship_controls( _events )
	end

	--Logic
	function player.logic( self, _deltaTime, _view )

		self.pos = move( {	dir = self.dir,
								pos = self.pos,
								hitbox = vec2( 2.5,2.5 ) },
							  _deltaTime )
		
		if self.fire == 1 then
			firing_function(self.pos, _deltaTime)
		end



		player.hitbox:teleport( vec2(self.pos:x(),self.pos:y()) )
		player.render_hitbox:update( player.hitbox )

		local _model = ship_turning(self.modelMatrix, self.dir, _deltaTime)
		local view = translate( _view,self.pos)

		self.ship:uniform( "Model", _model )
		self.ship:uniform( "View", view )
		self.ship:uniform( "NormalMatrix", normalSpace(view) )
		--self.ship:uniform( "NormalMatrix", normalSpace(ship.model) )
		self.ship:uniform( "LightPosition", vec3(0,0,500)*normalSpace(_view*_model) )



	end

	--Reload
	function player.reload(self)
		self.ship:reload()
		self.render_hitbox:reload()
	end

	--Render
	function player.render(self)
		self.ship:render()
		self.render_hitbox:render()
	end

	return player
end






























function make_ship( _name, _view, _fire, _move )

	if _fire == nil then
		error("no fire function included")
	end

	local settings = sys.load_ship(  _name )


	local ship = {}
	local firing_function = _fire
	local movement_function = _move

	local dead = false
	local health = settings.health

	ship.hitbox = collision.obb(settings.hitbox.x,settings.hitbox.y)
	ship.hitbox:z(-241.0)
	ship.vel = vec3(0)
	ship.dir = vec3(0)
	ship.pos = vec3(0)
	ship.fire = 0
	ship.modelMatrix = scale(
		translate( rotate(
			rotate(
				mat4(1.0),
				180,
				vec3( 1,0,0) ),
			180,
			vec3( 0,1,0) ),
			vec3(0,0,-0) ),
		vec3(5) )

	
	local render_hitbox = collision.visualHitbox( ship.hitbox )
	render_hitbox:uniform( "Colour", vec3(0.16863,1,0.1647) )
	render_hitbox:uniform( "ProjectionView", Projection*mat4(1.0) )


	local ship_model = load_player_ship( settings.model,_view, ship.modelMatrix )
	local ship_turning = turningClosure()


	function ship.damaged_by( _damage )
		render_hitbox:uniform( "Colour", vec3(1,0.16863,0.1647) )

		health = health - _damage
		if health <= 0 then
			dead = true
		end
	end
	--Event Handler
	function ship.handleEvents( self, _events )
		--self.dir, self.fire = player.ship_controls( _events )
	end

	--Logic
	function ship.logic( self, _deltaTime, _view )

		--movement_function(_deltaTime)
		--local move = movement_function(_deltaTime)-- self.dir*0.11*_deltaTime
		--self.pos = self.pos + move
		self.pos = movement_function(_deltaTime)

		self.hitbox:teleport( vec2(self.pos:x(),self.pos:y()) )
		render_hitbox:update( self.hitbox )

		local _model = ship_turning(self.modelMatrix, self.dir, _deltaTime)
		local view = translate( _view, self.pos )

		ship_model:uniform( "Model", _model )
		ship_model:uniform( "View", view )
		ship_model:uniform( "NormalMatrix", normalSpace(view) )
		ship_model:uniform( "NormalMatrix", normalSpace(_model) )
		ship_model:uniform( "LightPosition", vec3(0,0,500)*normalSpace(_view*_model) )
		render_hitbox:uniform( "Colour", vec3(0.16863,1,0.1647) )
	end

	--Reload
	function ship.reload(self)
		ship_model:reload()
		render_hitbox:reload()
	end

	--Render
	function ship.render(self)
		ship_model:render()
		render_hitbox:render()
	end

	function ship.is_dead(self)
		return dead
	end

	function ship.position(self, _pos)
		self.pos = self.pos + _pos
	end

	return ship
end

