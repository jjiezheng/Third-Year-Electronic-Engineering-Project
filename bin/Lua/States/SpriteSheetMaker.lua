dofile( include( "math" ) )

ships = SpriteSheet( "Attrition/Images/SpriteSheets/ships", "Attrition/Images/SpriteSheets/ships", 512, 512 )
bullets = SpriteSheet( "Attrition/Images/SpriteSheets/bullets", "Attrition/Images/SpriteSheets/bullets", 512, 512 )
explosion = SpriteSheet( "Attrition/Images/SpriteSheets/explosion", "Attrition/Images/SpriteSheets/explosion", 1024, 512 )
ui = SpriteSheet( "Attrition/Images/SpriteSheets/UI", "Attrition/Images/SpriteSheets/UI", 1024, 512 )
background = SpriteSheet( "Attrition/Images/SpriteSheets/backgrounds", "Attrition/Images/SpriteSheets/backgrounds", 2048, 1024 )
icons = SpriteSheet( "Attrition/Images/SpriteSheets/icons", "Attrition/Images/SpriteSheets/icons", 512, 512 )

function generateSpriteSheets()

	--[[ships:generate()
	bullets:generate()
	explosion:generate()
	ui:generate()
	background:generate()--]]
	icons:generate()
end


SpriteSheetMaker = {}
SpriteSheetMaker.gen = false

function SpriteSheetMaker.Start( self )

end

function SpriteSheetMaker.HandleEvents( self, _events )



	if events.wasKeyPressed( _events, key.space ) then
		self.gen = true
	end

	if events.wasKeyPressed( _events, key.f11 ) then
		return true
	else
		return false
	end
end

function SpriteSheetMaker.Logic( self, _deltaTime )

	if self.gen then
		generateSpriteSheets()
		self.gen = false
	end
end

function SpriteSheetMaker.Render( self )

end

function SpriteSheetMaker.Reload( self )


end
