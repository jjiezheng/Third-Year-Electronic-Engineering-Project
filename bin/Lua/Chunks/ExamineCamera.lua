dofile( include( "math" ) )

function ExamineCamera()

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
		local moveZ
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

		if events.isKeyDown( _events, key.lshift )  then
			moveZ = moveZ/ 10
		end

		if self.canMove then
			if self.mouseInput then
				self.translationVector = vec3( 0, 0, -moveZ )
				self.rotationYdiff = ( events.relativeMouseAngle( _events, 0 )*scale / 150.0 )
				self.rotationXdiff = ( events.relativeMouseAngle( _events, 1 )*scale / 150.0 )
			end
			events.resetMouse()
			self.mouseInput = false
		else
			self.translationVector = vec3(0, 0, -moveZ)
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


		self.view = translate( mat4(1.0), self.currentPosition )
		self.view = self.view*mat4Cast(self.rotationQuat)
	end

	return camera
end
