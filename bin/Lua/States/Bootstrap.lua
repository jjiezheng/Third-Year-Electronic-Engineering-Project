Bootstrap = {}

function Bootstrap.Start(self)
end

function Bootstrap.HandleEvents( self, _events )
	return false
end

function Bootstrap.Logic( self, _deltaTime )
	--setState( "Final_Game" )
	setState( "Final_Main_Menu" )
	--setState( "MeshViewer" )
end

function Bootstrap.Render(self)

end

function Bootstrap.Reload(self)

end
