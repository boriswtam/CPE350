
for i = 0, 5 do
		-- Opens a file in read
		file = io.open("Flight_" .. tostring(i), "a") --might want to change this to appending?

		-- sets the default output file as test.lua
		io.output(file)

		-- appends a word test to the last line of the file
		io.write("This is test file " .. tostring(i))

		-- closes the open file
		io.close(file)
end
