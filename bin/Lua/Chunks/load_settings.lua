
if sys.included == nil then
	sys.included = {}
end

if sys.included.load_settings == nil then
	local load_settings = sys.load_settings

	sys.load_settings = function( _file )
		dofile( load_settings(_file) )
	end

	sys.included.load_settings = true
end
