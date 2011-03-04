dofile( include( "math" ) )

Projection = perspective( 45.0, 16.0/10.0, 0.1, 1000.0 )

function normalSpace( _matrix )
	return inverseTranspose( mat3(_matrix) )
end

function make_rect( _width, _height )

	local width = _width or 1
	local height = _height or 1

	local _vertexBuffer = vertexArray()
	local _elementBuffer = intArray()

	_vertexBuffer:add( vertex(  height/2, -width/2, -0, 1 ) )
    _vertexBuffer:add( vertex(  height/2,  width/2, -0, 1 ) )
    _vertexBuffer:add( vertex( -height/2, -width/2, -0, 1 ) )
    _vertexBuffer:add( vertex( -height/2,  width/2, -0, 1 ) )

    _elementBuffer:add( 0 )
    _elementBuffer:add( 1 )
    _elementBuffer:add( 2 )
    _elementBuffer:add( 3 )

	return _vertexBuffer, _elementBuffer
end



Game = {}
function Game.Start( self )

	loadResources( "Attrition" )
	sys.title( "A Game" )

	self.rotateAngle = 0
	self.modelMatrix = scale( translate( rotate( mat4(1.0), 180,  vec3( 0,0,1) ), vec3(0,0,-0) ), vec3(2.5) )
	zDepth = 241
	self.View = translate( mat4(1.0), vec3(0,0,-zDepth) )
	self.borders = make_level_borders( self.View )

	self.player = make_player(self.View)

	--aButton = make_button( {640-0, 400-0}, 80, 35, "UI", "UIButton", { "UI.button1", "UI.button2", "UI.button3" } )
end

function Game.HandleEvents( self, _events )

	self.player:handleEvents( _events )

	if events.isKeyDown( _events, key[']'] ) then
		zDepth = zDepth+1
	elseif events.isKeyDown( _events, key['['] ) then
		zDepth = zDepth-1
	elseif events.isKeyDown( _events, key['space'] ) then
		Log("zDepth = " .. tostring(zDepth))
	end

	return false
end

function Game.Logic( self, _deltaTime )


	self.rotateAngle = self.rotateAngle + 0.05*_deltaTime
	self.View = translate( mat4(1.0), vec3(0,0,-zDepth) )
	--self.View = rotate( self.View, self.rotateAngle, vec3(0,1,0) )

	self.player:logic( _deltaTime, self.View )
	self.borders:logic( _deltaTime, self.View )
end

function Game.Render( self )
	self.borders:render()
	self.player:render()
	self.player.render_hitbox:render()
end

function Game.Reload( self )
	self.borders:reload()
	self.ship:reload()
end



--Closure for Setting how the ship behaves when it moves
function turningClosure()

	local animationTime = 125
	local moving = false
	local time = 0
	local current_direction = 1
	local direction = 1
	local starting_angle = 1
	local angle = 1

	--Behaviour which banks the ship in the direction it is turning
	return function( _player, _deltaTime )

		if _player.dir:x() > 0.01 or _player.dir:x() < -0.01 then

			if _player.dir:x() > 0 then direction = -1 else direction = 1 end

			if not moving or current_direction ~= direction then
				moving = true
				current_direction = direction
				time = 0
			end

			time = time + _deltaTime
			if time > animationTime then time = animationTime end
			angle = mix( starting_angle, 15*direction, smoothstep( 0, animationTime, time ) )

			return rotate( _player.ship.model, angle, vec3(0,0,-1) )
		else

			moving = false
			time = time - _deltaTime
			if time < 0 then time = 0 end
			angle = mix( 0, 15*direction, smoothstep( 0, animationTime, time ) )
			starting_angle = angle

			return  rotate( _player.ship.model, angle, vec3(0,0,-1) )
		end
	end
end
--Closure for Ship Controls
function controlsClosure( _controls )

	local dir = vec3(0)
	local controls = _controls or {}

	if controls.up == nil then	controls.up = key.w	end
	if controls.down == nil then controls.down = key.s	end
	if controls.left == nil then controls.left = key.a	end
	if controls.right == nil then controls.right = key.d end


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

		if dir:x() ~= 0 and dir:y() ~= 0 then
			dir = dir*0.71
		end

		return dir
	end
end
--Closure for Ship Movement
function movementClosure()

	local move = vec3(0)

	--Default Movement
	return function( _info, _deltaTime )

		move = _info.dir*0.11*_deltaTime
		_info.pos = _info.pos + move

		if _info.pos:x() + _info.hitbox:x() > 66 then _info.pos:set_x(66 - _info.hitbox:x())   end
		if _info.pos:x() - _info.hitbox:x() <  -66 then _info.pos:set_x(_info.hitbox:x() - 66)  end
		if _info.pos:y() + _info.hitbox:y() >  100 then _info.pos:set_y(100 - _info.hitbox:y())  end
		if _info.pos:y() - _info.hitbox:y() < -100 then _info.pos:set_y(_info.hitbox:y()-100) end

		return _info.pos
	end
end

function load_player_ship( _view, _modelMatrix )

	local ship = mesh()
	--ship:load( "HeadlessGiant", "CelShaded" )
	ship:load( "CameraRollAnim.3ds", "CelShaded" )
	--ship:load( "HeadlessGiant.lwo", "CelShaded" )
	ship.model = _modelMatrix

	ship:uniform( "Model", _modelMatrix )
	ship:uniform( "View", _view )
	ship:uniform( "Projection", Projection )
	ship:uniform( "NormalMatrix", normalSpace(ship.model) )

	ship:uniform( "LightPosition", vec3(0,0,500) )
	ship:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	ship:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	ship:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	ship:uniform( "Shininess", 100 )
	ship:depth( true )

	return ship
end


function make_player( _view )

	local player = {}

	player.vel = vec3(0)
	player.dir = vec3(0)
	player.pos = vec3(0)
	player.modelMatrix = scale( translate( rotate( rotate( mat4(1.0), 90,  vec3( 1,0,0) ), 180, vec3( 0,1,0) ), vec3(0,0,-0) ), vec3(2.5) )

	player.hitbox = collision.obb(5,5)
	player.render_hitbox = collision.visualHitbox( player.hitbox )
	player.render_hitbox:uniform( "Colour", vec3(0.16863,0.1451,0.1647) )
	player.render_hitbox:uniform( "ProjectionView", Projection*mat4(1.0) )
	player.render_hitbox:uniform( "Z", -241.0 )


	player.ship = load_player_ship( _view, player.modelMatrix )

	player.move = movementClosure()
	player.ship_turning = turningClosure()
	player.ship_controls = controlsClosure()


	--Event Handler
	function player.handleEvents( self, _events )
		self.dir = player.ship_controls( _events )
	end

	--Logic
	function player.logic( self, _deltaTime, _view )

		self.pos = self.move( {	dir = self.dir,
								pos = self.pos,
								hitbox = vec2( 2.5,2.5 ) },
							  _deltaTime )

		player.hitbox:teleport( vec2(self.pos:x(),self.pos:y()) )
		player.render_hitbox:update( player.hitbox )

		local _model = player.ship_turning(self, _deltaTime)
		local view = translate( _view,self.pos)

		self.ship:uniform( "Model", _model )
		self.ship:uniform( "View", view )
		self.ship:uniform( "NormalMatrix", normalSpace(view) )
		self.ship:uniform( "LightPosition", vec3(0,0,-500)*normalSpace(_view*_model) )

	end

	--Reload
	function player.reload(self)
		self.ship:reload()
	end

	--Render
	function player.render(self)
		self.ship:render()
	end

	return player
end

function make_level_borders( _view )

	local borderVertBuff, borderElemBuff = make_rect(200, 94)
	local border = {}

	border.left = mesh()
	border.left:shader("vbo"):vert_type("TriangleStrip")
	border.left:add("Position",borderVertBuff,borderElemBuff)

	border.left:uniform( "Colour", vec3(0.16863,0.1451,0.1647) )
	border.left:uniform( "Model", mat4(1.0) )
	border.left:uniform( "View", _view )
	border.left:uniform( "Projection", Projection )
	border.left:uniform( "NormalMatrix", mat4(1.0) )
	border.left:depth( true )

	border.right = mesh()
	border.right:shader("vbo"):vert_type("TriangleStrip")
	border.right:add("Position",borderVertBuff,borderElemBuff)

	border.right:uniform( "Colour", vec3(0.16863,0.1451,0.1647) )
	border.right:uniform( "Model", mat4(1.0) )
	border.right:uniform( "View", _view )
	border.right:uniform( "Projection", Projection )
	border.right:uniform( "NormalMatrix", mat4(1.0) )
	border.right:depth( true )


	local levelVertBuff, levelElemBuff = make_rect(400, 200)
	border.back = mesh()
	border.back:shader("vbo"):vert_type("TriangleStrip")
	border.back:add("Position",levelVertBuff,levelElemBuff)

	border.back:uniform( "Colour", vec3(0.8902,0.8470, 0.7137) )
	border.back:uniform( "Model", mat4(1.0) )
	border.back:uniform( "View", _view )
	border.back:uniform( "Projection", Projection )
	border.back:uniform( "NormalMatrix", mat4(1.0) )
	border.back:depth( true )



	function border.logic( self, _deltaTime, _view )

		self.left:uniform( "View", translate( _view,vec3(85,0,100) ) )
		self.right:uniform( "View", translate( _view,vec3(-85,0,100) ) )
		self.back:uniform( "View", translate( _view,vec3(0,0,-100) ) )
	end

	function border.reload(self)
		self.right:reload()
		self.left:reload()
		self.back:reload()
	end

	function border.render(self)
		self.right:render()
		self.left:render()
		self.back:render()
	end

	return border
end

function make_button( _position, _width, _height, _texture, _shader, _sprites )
	local button = {}

	button.hovering = false
	button.pressedLastFrame = false
	button.pressed = false
	button.hover = 0.0
	button.timeHover = 0.0

	button.hitbox = Poly( _width, _height, true )
	button.hitbox:transform( Vector(_position[1],_position[2]), 0 )

	for i, v in ipairs(_sprites) do

		if i == 1 then
			button.sprite = Sprite3D( _texture, v, _shader )
		else
			button.sprite:addSprite( _texture, v, i-1 )
		end
	end
	button.sprite:uniform( "offset", vec3( (_position[1]-640),
										   (397-_position[2]),
										   -1000.0/1.049 ) )

	button.sprite:uniform( "angle", 0.0 ):uniform( "hover", 0.0 ):uniform( "pressed", 0.0 ):uniform( "ProjectionView", Projection )

	function button.handleEvents( self, _events )

		if not self.hovering and self.hitbox:containsPoint( events.getMousePosition( _events, true ) ) then
			self.hovering = true
			self.timeHover = 0
		elseif self.hovering and not self.hitbox:containsPoint( events.getMousePosition( _events, true ) ) then
			self.hovering = false
			self.timeHover = 0
		end

		if events.isButtonDown( _events, mouse.left ) then
			self.pressed = true
		elseif events.isButtonUp( _events, mouse.left ) then
			self.pressed = false
		end
	end

	function button.logic( self, _deltaTime )

		if self.hovering then

			if self.timeHover < 1000 then
				self.timeHover = self.timeHover + _deltaTime/4
				self.hover = smoothstep( 0, 100.0, self.timeHover)
				self.sprite:uniform( "hover", self.hover )
			end

		else
			if self.timeHover < 1000 then
				self.timeHover = self.timeHover + _deltaTime/4
				self.hover = smoothstep( 0, 100.0, self.timeHover)
				self.sprite:uniform( "hover", 1.0-self.hover )
			end

		end

		if self.hovering then
			if self.pressed then
				self.sprite:uniform( "pressed", 1.0 )
			else
				self.sprite:uniform( "pressed", 0.0 )
			end
		else
			self.sprite:uniform( "pressed", 0.0 )
		end

	end


	function button.render(self)
		self.sprite:render()
		self.hitbox:render(Colour(0.0,1.0,0.0,0.2),3)
	end

	return button
end
