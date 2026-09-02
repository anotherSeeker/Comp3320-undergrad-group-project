const MAX_SPEED = 10
const MIN_SPEED = 2
const key_map = {
	[enum.keycode.W] = false,
	[enum.keycode.A] = false,
	[enum.keycode.S] = false,
	[enum.keycode.D] = false,
	[enum.keycode.Q] = false,
	[enum.keycode.E] = false,
	[enum.keycode.LEFT_SHIFT] = false,
}

local mousedown = false
local speed = MIN_SPEED

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
	speed = key_map[enum.keycode.LEFT_SHIFT] == true and MIN_SPEED or MAX_SPEED

	if key_map[enum.keycode.W] then
		camera.move(0, 0, -speed * dt)
	end

	if key_map[enum.keycode.S] then
		camera.move(0, 0, speed * dt)
	end

	if key_map[enum.keycode.A] then
		camera.move(-speed * dt, 0, 0)
	end

	if key_map[enum.keycode.D] then
		camera.move(speed * dt, 0, 0)
	end

	if key_map[enum.keycode.Q] then
		camera.move(0, speed * dt, 0)
	end

	if key_map[enum.keycode.E] then
		camera.move(0, -speed * dt, 0)
	end
end)

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
