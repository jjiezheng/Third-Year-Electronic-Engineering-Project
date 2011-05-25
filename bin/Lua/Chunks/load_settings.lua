
if sys.included == nil then
	sys.included = {}
	ship_settings = {}
	ships_loaded = {}
end

if sys.included.load_settings == nil then
	local load_settings = sys.load_settings


	sys.load_settings = function( _file )
		dofile( load_settings(_file) )
	end

	local load_ship_closure = function()
		local ship_settings = {}
		local ships_loaded = {}

		return function(_ship_name )
		
			if ships_loaded[_ship_name] == nil then
				ships_loaded[_ship_name] = true

				local ship_path = "Ships\\".._ship_name
				local full_path = load_settings(ship_path)

				local f = assert(io.open(full_path, "r"))
				local t = f:read("*all")
				f:close()

				if type(t) == "string" then
					t = "return "..t
				end
				ship_settings[_ship_name] = loadstring(t)()
			end
			return ship_settings[_ship_name]
		end
	end
	sys.load_ship = load_ship_closure()

	sys.included.load_settings = true
end

function serialize(o)
	local t = type(o)

	if t == "number" then
		return tostring(o)

	elseif t == "string" then
		return string.format("%q", o)

	elseif t == "table" then
		local result = "\n{"
		local nextIndex = 1
		local first = true
		for k, v in pairs(o) do
			if first then
				first = false
				result = result .. "\n\t"
			else
				result = result .. ", " .. "\n\t"
			end
			if type(k) == "number" and k == nextIndex then
				nextIndex = nextIndex + 1
			else
				if type(k) == "string" and string.find(k, "^[_%a][_%w]*$") then
					result = result .. k
				else
					result = result .. "[" .. serialize(k) .. "]"
				end
				result = result .. " = "
			end
			result = result .. serialize(v)
		end
		result = result .. "\n}"
		return result
	else
		return tostring(o)
	end
end
