input.connectKeyPress(function(value: number)
	const keycode: string = string.char(value)

	if keycode == "W" then
		camera.move(0, 0, -1)
	end

	if keycode == "S" then
		camera.move(0, 0, 1)
	end

	if keycode == "A" then
		camera.move(-1, 0, 0)
	end

	if keycode == "D" then
		camera.move(1, 0, 0)
	end
end)

input.connectKeyLifted(function(value: number)
	print("key lifted")
end)

runservice.connectPreRender(function(dt: number)
	print(dt)
end)
