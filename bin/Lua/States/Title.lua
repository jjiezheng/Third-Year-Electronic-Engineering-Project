--dofile( include( "firstPersonCamera" ) )

Title = {}

function Title.Start(self)
	local sin, cos = math.sin, math.cos
	math.sin = function (x) return sin(math.rad(x)) end
	math.cos = function (x) return cos(math.rad(x)) end
	math.uQuat = function() return quat( 1.0, vec3(0.0) ) end
	math.uMat3 = function() return mat3( 1.0 ) end
	math.uMat4 = function() return mat4( 1.0 ) end

	self.Name = SString( "GAME ENGINE WITH HAPTICS", "Unsteady", Vector( 640, 30 ), 11, SString.alignCentre )
    self.Game = SString( "", "Unsteady", Vector( 640, 425 ), 11, SString.alignCentre )
	self.Name:setColour( Colour( 0.0, 0.0, 0.0, 1.0 ) )
	self.Game:setColour( Colour( 0.7, 0.7, 0.7, 0.8 ) )


	Projection = perspective( 65.0, 16.0/10.0, 0.1, 100.0 )
	up = vec3( 0.0, -1.0, 0.0 )
	lightVector = vec3(0.0,0.0,6)

	self.camera = firstPersonCamera()

	self.Objects = genCubes()
	self.Objects:logic(self.camera)

	self.Slides = makeSlides( { "Slide_1", "Slide_1", "Slide_1", "Slide_1" } )
end

function Title.HandleEvents( self, _events )

	self.camera:handleEvents( _events )
	self.Slides:handleEvents( _events )

	if events.wasKeyPressed( _events, key.f10 ) then
		setState( "Presentation" )
	end

	if events.wasKeyPressed( _events, key.f11 ) then
		return true
	else
		return false
	end
end

function Title.Logic( self, _deltaTime )
	self.camera:logic( _deltaTime )
	self.Objects:logic( self.camera )
	self.Slides:logic()

	lightVector = lightVector + vec3(0.0,0.0,-0.2)*( _deltaTime/40 )
	--Objects[3].Model = translate(Objects[3].Model,lightVector)

	for i, v in ipairs(self.Objects) do
		--v.mesh:uniform("LightPosition",lightVector)
	end
end

function Title.Render(self)
	self.Name:render(3)
	self.Game:render(3)

	self.Objects:render()
	self.Slides:render()
end

function Title.Reload(self)
	self.self.Name:reload()
	Game:reload()

	self.Objects:reload()
	self.Slides:reload()
end



function makeCube()

	local _vertexBuffer = vertexArray()
	local _normalBuffer = vertexArray()
	local _elementBuffer = intArray()

	_vertexBuffer:add( vertex(  1.0, -1.0, -1.0, 1.0 ) )
    _vertexBuffer:add( vertex(  1.0,  1.0, -1.0, 1.0 ) )
    _vertexBuffer:add( vertex( -1.0, -1.0, -1.0, 1.0 ) )
    _vertexBuffer:add( vertex( -1.0,  1.0, -1.0, 1.0 ) )
	_vertexBuffer:add( vertex(  1.0, -1.0,  1.0, 1.0 ) )
    _vertexBuffer:add( vertex(  1.0,  1.0,  1.0, 1.0 ) )
    _vertexBuffer:add( vertex( -1.0, -1.0,  1.0, 1.0 ) )
    _vertexBuffer:add( vertex( -1.0,  1.0,  1.0, 1.0 ) )

	_normalBuffer:add( normalise( vertex(  1.0, -1.0, -1.0, 0.0 ) ) )
    _normalBuffer:add( normalise( vertex(  1.0,  1.0, -1.0, 0.0 ) ) )
    _normalBuffer:add( normalise( vertex( -1.0, -1.0, -1.0, 0.0 ) ) )
    _normalBuffer:add( normalise( vertex( -1.0,  1.0, -1.0, 0.0 ) ) )
	_normalBuffer:add( normalise( vertex(  1.0, -1.0,  1.0, 0.0 ) ) )
    _normalBuffer:add( normalise( vertex(  1.0,  1.0,  1.0, 0.0 ) ) )
    _normalBuffer:add( normalise( vertex( -1.0, -1.0,  1.0, 0.0 ) ) )
    _normalBuffer:add( normalise( vertex( -1.0,  1.0,  1.0, 0.0 ) ) )


    _elementBuffer:add( 0 )
    _elementBuffer:add( 1 )
    _elementBuffer:add( 2 )
    _elementBuffer:add( 3 )
    _elementBuffer:add( 6 )
    _elementBuffer:add( 7 )
    _elementBuffer:add( 4 )
    _elementBuffer:add( 5 )
    _elementBuffer:add( 0 )
    _elementBuffer:add( 1 )
    _elementBuffer:add( 1 )
    _elementBuffer:add( 5 )
    _elementBuffer:add( 3 )
    _elementBuffer:add( 7 )
    _elementBuffer:add( 7 )
    _elementBuffer:add( 0 )
    _elementBuffer:add( 0 )
    _elementBuffer:add( 4 )
    _elementBuffer:add( 2 )
    _elementBuffer:add( 6 )

	return _vertexBuffer, _normalBuffer, _elementBuffer

end

function genCubes()

	local _objects = {}
	local vertexBuffer, normalBuffer, elementBuffer = makeCube()

	_objects["show"] = true
	_objects.next = function(self) return (#self+1) end

	_objects.render = function(self)
		if self["show"] then
			for i, v in ipairs(self) do
				v.mesh:render()
			end
		end
	end

	_objects.reload = function(self)
		for i, v in ipairs(self) do
			v.mesh:reload()
		end
	end

	_objects.logic = function(self, _camera)
		for i, v in ipairs(self) do
			v.View = _camera.view
			--v.Normal = transpose( inverse( mat3(Objects[C].View*Objects[C].Model) ) )

			v.mesh:uniform("Model",v.Model):uniform("View",v.View)
			v.mesh:uniform("Projection",Projection):uniform("NormalMatrix",v.Normal)
		end
	end


	C = _objects:next()
	_objects[C] = {}
	_objects[C].mesh = mesh()

	_objects[C].Model = translate( mat4(1.0),vec3(-7.0,-0.0,-8.0) )
	_objects[C].View = math.uMat4()
	_objects[C].Normal = transpose( inverse( mat3(_objects[C].View*_objects[C].Model) ) )



	_objects[C].mesh:shader("BasicVbo"):vert_type("TriangleStrip")
	_objects[C].mesh:add("Position",vertexBuffer,elementBuffer):add("Normal",normalBuffer)

	_objects[C].mesh:uniform("DiffuseMaterial", vec3(0.095, 0.243, 0.4) )
	_objects[C].mesh:uniform("LightPosition", vec3(0.0,0.0,-7) )
	_objects[C].mesh:uniform("AmbientMaterial", vec3(0.04) )
	_objects[C].mesh:uniform("SpecularMaterial", vec3(0.5) )
	_objects[C].mesh:uniform("Shininess", 50 )

	C = _objects:next()
	_objects[C] = {}
	_objects[C].mesh = mesh()

	_objects[C].Model = translate( mat4(1.0),vec3( 7.0,-0.0,-8.0) )
	_objects[C].View = math.uMat4()
	_objects[C].Normal = transpose( inverse( mat3(_objects[C].View*_objects[C].Model) ) )

	_objects[C].mesh:shader("BasicVbo"):vert_type("TriangleStrip")
	_objects[C].mesh:add("Position",vertexBuffer,elementBuffer):add("Normal",normalBuffer)


	_objects[C].mesh:uniform("DiffuseMaterial", vec3(0, 0.75, 0.75) )
	_objects[C].mesh:uniform("LightPosition", vec3(0.0,0.0,0.1) )
	_objects[C].mesh:uniform("AmbientMaterial", vec3(0.04) )
	_objects[C].mesh:uniform("SpecularMaterial", vec3(0.5) )
	_objects[C].mesh:uniform("Shininess", 50 )

	C = _objects:next()
	_objects[C] = {}
	_objects[C].mesh = mesh()

	_objects[C].Model = scale( translate( mat4(1.0),vec3( 0.0,-0.0,-12.0) ), vec3(2.2) )
	_objects[C].View = math.uMat4()
	_objects[C].Normal = transpose( inverse( mat3(_objects[C].View*_objects[C].Model) ) )

	_objects[C].mesh:shader("BasicVbo"):vert_type("TriangleStrip")
	_objects[C].mesh:add("Position",vertexBuffer,elementBuffer):add("Normal",normalBuffer)

	_objects[C].mesh:uniform("DiffuseMaterial", vec3(0.095, 0.243, 0.4) )
	_objects[C].mesh:uniform("LightPosition", vec3(0.0,0.0,-7) )
	_objects[C].mesh:uniform("AmbientMaterial", vec3(0.04) )
	_objects[C].mesh:uniform("SpecularMaterial", vec3(0.5) )
	_objects[C].mesh:uniform("Shininess", 500 )

	return _objects
end


function makeSlide()


	local _vertexBuffer = vertexArray()
	local _textureBuffer = vertexArray()
	local _elementBuffer = intArray()

	_vertexBuffer:add( vertex(  1.0, -1.0, -1.0, 1.0 ) )
    _vertexBuffer:add( vertex(  1.0,  1.0,  -1.0, 1.0 ) )
    _vertexBuffer:add( vertex( -1.0, -1.0,  -1.0, 1.0 ) )
    _vertexBuffer:add( vertex( -1.0,  1.0,  -1.0, 1.0 ) )

	_textureBuffer:add( vertex( 1.0, 1.0, 0.0, 0.0 ) )
    _textureBuffer:add( vertex( 1.0, 0.0, 0.0, 0.0 ) )
    _textureBuffer:add( vertex( 0.0, 1.0, 0.0, 0.0 ) )
    _textureBuffer:add( vertex( 0.0, 0.0, 0.0, 0.0 ) )


    _elementBuffer:add( 0 )
    _elementBuffer:add( 1 )
    _elementBuffer:add( 2 )
    _elementBuffer:add( 3 )


	return _vertexBuffer, _textureBuffer, _elementBuffer

end

function makeSlides( _slideNames)

	local slideVertBuff, slideTexBuff, slideElemBuff = makeSlide()
	local localSlides = {}

	localSlides.view = lookAt( vec3( 0.0,0.0, 0.0 ),
							   vec3( 0.0,0.0,-1.0 ),
							   vec3( 0.0,1.0, 0.0 ))

	localSlides.show = true


	for i, v in ipairs(_slideNames) do

		local vector = vec3( (2.5)*(i-1), 0.0, -2.5)
		local _projection = perspective( 35.0, 16.0/10.0, 0.1, 100.0 )

		localSlides[i] = mesh()
		localSlides[i]:shader("TexturedVbo"):vert_type("TriangleStrip")
		localSlides[i]:add("Position",slideVertBuff,slideElemBuff):add("Tex",slideTexBuff)

		localSlides[i]:uniform( "Model",translate( mat4(1.0),vector) ):uniform( "View",localSlides.view )
		localSlides[i]:uniform("Projection",_projection)
		localSlides[i]:uniform("Texture0",v)
	end

	localSlides.handleEvents = function (self, _events)

		if events.wasKeyPressed( _events, key.z ) then
			self.view = translate( self.view, vec3(2.5,0.0,0.0) )

		elseif events.wasKeyPressed( _events, key.c ) then
			self.view = translate( self.view, vec3(-2.5,0.0,0.0) )
		end

		if events.wasKeyPressed( _events, key.x ) then

			if self.show then
				self.show = false
			else
				self.show = true
			end
		end
	end

	localSlides.logic = function (self)
		for i, v in ipairs(self) do
			v:uniform("View",self.view)
		end
	end

	localSlides.render = function(self)
		if self.show then
			for i, v in ipairs(self) do
				v:render()
			end
		end
	end

	localSlides.reload = function(self)
		for i, v in ipairs(self) do
			v:reload()
		end
	end

	return localSlides

end

function firstPersonCamera()

	local camera = {}

	camera.view = lookAt( vec3( 0.0,0.0, 0.0 ),
				  vec3( 0.0,0.0,-1.0 ),
			      vec3( 0.0,1.0, 0.0 ))


	camera.rotationQuat = math.uQuat()


	camera.translationVector = vec3( 0.0 )
	camera.currentPosition = vec3( 0.0, 0.0, -10.0 )
	camera.mouseVector = vec3( 0.0 )


	camera.rotationX = 0
	camera.rotationY = 0
	camera.rotationXdiff = 0
	camera.rotationYdiff = 0

	camera.mouseInput = true
	camera.canMove = false



	camera.handleEvents = function( self, _events )
		local moveZ, moveY
		local scale = 1.5

		if events.isKeyDown( _events, key.w ) and events.isKeyDown( _events, key.s ) then
			moveZ = 0.0
		elseif events.isKeyDown( _events, key.w ) then
			moveZ = -1.0
		elseif events.isKeyDown( _events, key.s ) then
			moveZ = 1.0
		else
			moveZ = 0.0
		end

		if events.isKeyDown( _events, key.a ) and events.isKeyDown( _events, key.d ) then
			moveY = 0.0
		elseif events.isKeyDown( _events, key.a ) then
			moveY = 1.0
		elseif events.isKeyDown( _events, key.d ) then
			moveY = -1.0
		else
			moveY = 0.0
		end

		if self.canMove then
			if self.mouseInput then
				local x = math.sin( self.rotationY )*moveZ + math.sin( self.rotationY + 90 )*moveY
				local z = math.cos( self.rotationY )*moveZ + math.cos( self.rotationY + 90 )*moveY
				self.translationVector = vec3( x, 0.0, -z )
				self.rotationYdiff = ( events.relativeMouseAngle( _events, 0 )*scale / 100.0 )
				self.rotationXdiff = ( events.relativeMouseAngle( _events, 1 )*scale / 100.0 )
			end
			events.resetMouse()
			self.mouseInput = false
		else
			self.translationVector = vec3(0.0)
		end

		if events.wasKeyPressed( _events, key.q ) then
			events.toggleMouse()
			if self.canMove then
				self.canMove = false
			else
				self.canMove = true
			end
		end
	end

	camera.logic = function( self, _deltaTime )
		self.mouseInput = true

		self.rotationY = self.rotationY + self.rotationYdiff*_deltaTime
		self.rotationX = self.rotationX + self.rotationXdiff*_deltaTime
		self.rotationXdiff = 0
		self.rotationYdiff = 0

		self.movement = self.translationVector*( _deltaTime/40 )
		self.currentPosition = self.currentPosition + self.movement


		self.rotationQuatX = rotateQuat( math.uQuat(), self.rotationX, normalise( vec3( 1.0, 0.0, 0.0 ) ) )
		self.rotationQuatY = rotateQuat( math.uQuat(), self.rotationY, normalise( vec3( 0.0, 1.0, 0.0 ) ) )
		self.rotationQuat = cross( camera.rotationQuatX, self.rotationQuatY )

		self.view = mat4Cast(self.rotationQuat)
		self.view = translate( self.view, self.currentPosition )
	end

	return camera
end
