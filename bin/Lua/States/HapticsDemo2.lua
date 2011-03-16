HapticsDemo2 = {}

function HapticsDemo2.Start(self)
	sys.title( "Haptics" )

	haptics.init()
	haptics.workspace()
end

function HapticsDemo2.HandleEvents( self, _events )



	return false
end

function HapticsDemo2.Logic( self, _deltaTime )

end

function HapticsDemo2.Render(self)
	haptics.run()
end

function HapticsDemo2.Reload(self)

end
