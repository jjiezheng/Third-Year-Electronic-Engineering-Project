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
