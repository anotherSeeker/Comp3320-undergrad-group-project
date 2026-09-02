const SPEED = 10

const key_map = {
	[enum.keycode.W] = false,
	[enum.keycode.A] = false,
	[enum.keycode.S] = false,
	[enum.keycode.D] = false,
	[enum.keycode.Q] = false,
	[enum.keycode.E] = false,
}

input.connectKeyPress(function(keycode: number)
	if key_map[keycode] ~= nil then
		key_map[keycode] = true
	end
end)

input.connectKeyLifted(function(keycode: number)
	if key_map[keycode] ~= nil then
		key_map[keycode] = false
	end
end)

runservice.connectPreRender(function(dt: number)
	if key_map["W"] then
		camera.move(0, 0, -SPEED * dt)
	end

	if key_map["S"] then
		camera.move(0, 0, SPEED * dt)
	end

	if key_map["A"] then
		camera.move(-SPEED * dt, 0, 0)
	end

	if key_map["D"] then
		camera.move(SPEED * dt, 0, 0)
	end

	if key_map["Q"] then
		camera.move(0, SPEED * dt, 0)
	end

	if key_map["E"] then
		camera.move(0, -SPEED * dt, 0)
	end
end)

local mousedown = false

input.connectMousePress(function(key: number)
	if key ~= 1 then
		return
	end
	mousedown = true
end)

input.connectMouseLifted(function(key: number)
	if key ~= 1 then
		return
	end
	mousedown = false
end)

input.connectMouseMove(function(mouseX: number, mouseY: number, deltaX: number, deltaY: number)
	if mousedown == false then
		return
	end

	camera.rotate(-deltaX * 0.005, -deltaY * 0.005, 0)
end)
