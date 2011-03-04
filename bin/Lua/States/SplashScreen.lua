SplashScreen = {}

function SplashScreen.Start(self)
	f = assert(io.open("../test.txt", "w"))
end

function SplashScreen.HandleEvents( self, _events )

	readInput( _events )

	if events.wasKeyPressed( _events, key.f11 ) then
		return true
	else
		return false
	end
end

function SplashScreen.Logic( self, _deltaTime )
end

function SplashScreen.Render(self)

end

function SplashScreen.Reload(self)

end



function readInput( _events )

	if events.getKeyPressed( _events ) then
		f:write( events.getKeyPressed( _events ) )
	end
end
