dofile( include( "math" ) )

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
	camera.canMove = true



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
			moveY = 0.5
		elseif events.isKeyDown( _events, key.d ) then
			moveY = -0.5
		else
			moveY = 0.0
		end

		if self.canMove then
			if self.mouseInput then
				local x = math.sin( self.rotationY )*moveZ + math.sin( self.rotationY + 90 )*moveY
				local z = math.cos( self.rotationY )*moveZ + math.cos( self.rotationY + 90 )*moveY
				self.translationVector = vec3( x, 0.0, -z )
				self.rotationYdiff = ( events.relativeMouseAngle( _events, 0 )*scale / 150.0 )
				self.rotationXdiff = ( events.relativeMouseAngle( _events, 1 )*scale / 150.0 )
			end
			events.resetMouse()
			self.mouseInput = false
		else
			self.translationVector = vec3(0.0)
		end

		if events.wasKeyPressed( _events, key.q ) then
			events.showMouse( self.canMove )
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
