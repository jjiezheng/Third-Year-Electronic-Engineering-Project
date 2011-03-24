dofile( include( "math" ) )
dofile( include( "save" ) )
dofile( include( "FreeFormCamera" ) )
dofile( include( "make_earth" ) )
dofile( include( "strings" ) )


Projection = perspective( 45.0, 16.0/10.0, 1, 10000.0 )



PlanetTest = {}
function PlanetTest.Start( self )
	sys.title( "Planet Render Test" )

	self.camera = FreeFormCamera()
	self.world = make_earth()
	self.angle = 0
end

function PlanetTest.HandleEvents( self, _events )

	self.camera:handleEvents( _events )
	
	return false
end

function PlanetTest.Logic( self, _deltaTime )

	self.camera:logic( _deltaTime )
	self.angle = self.angle + 0.0005*_deltaTime

	local cameraPos = vec3(0,0,35) - self.camera.currentPosition


	self.world:uniform( "Model", 
		rotate( rotate( 
			mat4(1.0), 23.44, vec3(0,0,-1) ), 
			self.angle,vec3(0,1,0) ) )

	self.world:uniform( "View", 
		self.camera.view*translate( 
			mat4(1), vec3(0,0,-35) ) )

	self.world:uniform( "v3CameraPos", cameraPos )
	self.world:uniform( "fCameraHeight", length(cameraPos) )
	self.world:uniform( "fCameraHeight2", length(cameraPos)^2 )
end

function PlanetTest.Render( self )

	self.world:render()

end

function PlanetTest.Reload( self )
	self.world:reload()
end