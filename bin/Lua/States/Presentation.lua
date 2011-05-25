dofile( include( "math" ) )
Presentation = {}

----------------------------------------------------------------------
--Presentation Constructor								   			--
----------------------------------------------------------------------
--Runs once.														--
----------------------------------------------------------------------
function Presentation.Start( self )

	sys.title( "Christmas Presentation" )
	loadResources( "Presentation" )
	self.Projection = perspective( 65.0, 16.0/10.0, 0.1, 1000.0 )
	self.Slides = makeSlides( {
								"Slide_1",
								"Slide_2",
								"Slide_3",
								"Slide_6",
								"Slide_4",
								"Slide_5",
								"Slide_7",
								"Slide_8",
								"Slide_12",
								"Slide_9",
								"Slide_10",
								"Slide_11",
								"Slide_13"
							  },
							  vec3(0.0,0.0,-2.5),
							  vec3(2.5,0,0), true )
end

----------------------------------------------------------------------
--Presentation Logic function										--
----------------------------------------------------------------------
--Runs at the start of every frame.									--
--_events is a C struct containing all events that have happened and--
--is to be passed into event functions.								--
----------------------------------------------------------------------
function Presentation.HandleEvents( self, _events )

	self.Slides:handleEvents( _events )

	if events.wasKeyPressed( _events, key.f11 ) then
		return true
	else
		return false
	end
end

----------------------------------------------------------------------
--Presentation Logic function										--
----------------------------------------------------------------------
--Runs once every frame after Presentation.HandleEvents				--
--_deltaTime is the timestep since the last frame in milliseconds	--
----------------------------------------------------------------------
function Presentation.Logic( self, _deltaTime )

	self.Slides:logic(_deltaTime)
end

----------------------------------------------------------------------
--Presentation Render function										--
----------------------------------------------------------------------
--Runs at the end of every frame after Presentation.Logic			--
----------------------------------------------------------------------
function Presentation.Render( self )

	self.Slides:render()
end

----------------------------------------------------------------------
--Presentation Reload function										--
----------------------------------------------------------------------
--Runs when OpenGL context is reset.								--
----------------------------------------------------------------------
function Presentation.Reload( self )

	self.Slides:reload()
end



--returns the vertices for a cube
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


--returns the vertices for a slide
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


--Makes the slides class and returns it.
function makeSlides( _slideNames, _initalVector, _diffVector, _makeExtra)

	local slideVertBuff, slideTexBuff, slideElemBuff = makeSlide()
	local vertexBuffer, normalBuffer, elementBuffer = makeCube()
	local localSlides = {}

	localSlides.viewReset = lookAt( vec3( 0.0,0.0, 0.0 ),
									vec3( 0.0,0.0,-1.0 ),
									vec3( 0.0,1.0, 0.0 ))
	localSlides.view = localSlides.viewReset

	localSlides.moving = false
	localSlides.current = vec3(0)
	localSlides.old = vec3(0)
	localSlides.new = vec3(0)
	localSlides.offset = vec3(0)
	localSlides.time = 0
	localSlides.slideNum = 1
	localSlides.changeShader = false


	local _projection = perspective( 35.0, 16.0/10.0, 0.1, 100.0 )
	local n = #_slideNames

	for i, v in ipairs(_slideNames) do

		local vector = _initalVector + _diffVector*(i-1) + vec3(0,0,0.02)
		localSlides[i] = mesh()
		localSlides[i]:writeToDepth( true):depth(true)
		localSlides[i]:shader("TexturedVbo"):vert_type("TriangleStrip")
		localSlides[i]:add("Position",slideVertBuff,slideElemBuff):add("UV_0",slideTexBuff)

		localSlides[i]:uniform( "Model",translate( mat4(1.0),vector) ):uniform( "View",localSlides.view )
		localSlides[i]:uniform("Projection",_projection)
		localSlides[i]:texture("Texture0",v)

		if _makeExtra then
			localSlides[n+i] = mesh()
			localSlides[n+i]:writeToDepth( true):depth(true)
			localSlides[n+i].Model = translate( mat4(1.0), _diffVector*(i-1) + vec3(0,0,-4.5) )

			localSlides[n+i]:shader("vbo"):vert_type("TriangleStrip")
			localSlides[n+i]:add("Position",vertexBuffer,elementBuffer)

			localSlides[n+i]:uniform("Projection",_projection)
			localSlides[n+i]:uniform("View",localSlides.view)
			localSlides[n+i]:uniform("Model",localSlides[n+i].Model)

			localSlides[n+i]:uniform("Colour",vec3(0.67, 0, 0) )
		end
	end

	if _makeExtra then

		local C = (#_slideNames*2 + 1)
		localSlides.ex1 = C
		localSlides[C] = mesh()
		localSlides[C].Model = scale( translate( mat4(1.0), vec3((2.5*6),0.0,-2.5) ), vec3(0.2) )

		localSlides[C]:shader("vbo"):vert_type("TriangleStrip")
		localSlides[C]:add("Position",vertexBuffer,elementBuffer)

		localSlides[C]:uniform("Projection",_projection)
		localSlides[C]:uniform("View",localSlides.view)
		localSlides[C]:uniform("Model",localSlides[C].Model)

		localSlides[C]:uniform("Colour",vec3(1, 1, 0) )


		--[[localSlides.ex2 = Sprite()
		localSlides.ex2:setTexture( "ships","Basic" )
		localSlides.ex2:setTexCoords( "ships.Dasein1" )

		localSlides.ex2:setPriority( 16 )
		localSlides.ex2:setCoords( Poly( 200,200,true ) )
		localSlides.ex2:transform( Vector(640,400), 0, true )--]]
	end

	-------------------------------------------------------------
	--handlEvents											   --
	-------------------------------------------------------------
	localSlides.handleEvents = function (self, _events)

		if events.wasKeyPressed( _events, key.z ) then
			self.time = 0
			self.old = self.current
			self.new = self.new + vec3(2.5,0.0,0.0)
			self.moving = true
			self.slideNum = self.slideNum - 1

			if self.slideNum == 8 or self.slideNum == 9 then
				self.changeShader = true
			end

		elseif events.wasKeyPressed( _events, key.c ) then
			self.time = 0
			self.old = self.current
			self.new = self.new + vec3(-2.5,0.0,0.0)
			self.moving = true
			self.slideNum = self.slideNum + 1

			if self.slideNum == 8 or self.slideNum == 9 then
				self.changeShader = true
			end

		elseif events.wasKeyPressed( _events, key.f ) then
			self.offset = self.offset + vec3(0,0,0.1 )

		elseif events.wasKeyPressed( _events, key.v ) then
			self.offset = self.offset + vec3(0,0,-0.1 )
		end
	end

	-------------------------------------------------------------
	--logic											   		   --
	-------------------------------------------------------------
	localSlides.logic = function (self,_deltaTime)

		if self.time < 1000.0 and self.moving then
			self.time = self.time + _deltaTime/4
			self.current = mix( self.old, self.new, smoothstep( 0, 100.0, self.time) )
		else
			self.time = 0
			self.moving = false
		end

		self.view = translate( self.viewReset, self.current+self.offset )
		for i, v in ipairs(self) do
			v:uniform("View",self.view)
		end

		if _makeExtra then
			local rotation = rotateQuat( math.uQuat(), 2, normalise(vec3(0.5, 0.2, 0.8)) )

			self[self.ex1].Model = self[self.ex1].Model*mat4Cast(rotation)
			self[self.ex1]:uniform("Model",self[self.ex1].Model)

			if self.changeShader then
				self.changeShader = false
--[[
				if self.slideNum == 8 then
					localSlides.ex2:setTexture( "ships","Basic" )
				elseif self.slideNum == 9 then
					localSlides.ex2:setTexture( "ships","Convol" )
				end--]]
			end
		end
	end

	-------------------------------------------------------------
	--render											   	   --
	-------------------------------------------------------------
	localSlides.render = function(self)
		for i, v in ipairs(self) do
			v:render()
		end

		if self.slideNum == 8 or self.slideNum == 9 then
			--self.ex2:render(3)
		end
	end

	-------------------------------------------------------------
	--reload											   	   --
	-------------------------------------------------------------
	localSlides.reload = function(self)
		for i, v in ipairs(self) do
			v:reload()
		end

		--localSlides.ex2:reload()
	end


	return localSlides
end
