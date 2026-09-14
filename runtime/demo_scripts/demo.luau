--!strict
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
	[enum.keycode.SPACE] = false,
}

const camera_distance = 5
local camera_offset = vector.create(0, 1.5, camera_distance)

camera.setPosition(camera_offset)
camera.setOrientation(vector.create(0, math.pi, 0))

local mousedown = false
local speed = MIN_SPEED

local a =
	createObject("../assets/meshes/uvsphere.obj", "../assets/shaders/default.vert", "../assets/shaders/default.frag")

local b =
	createObject("../assets/meshes/suzanne.obj", "../assets/shaders/default.vert", "../assets/shaders/default.frag")

b:setPosition(vector.create(5, 0, 0))
b:setOrientation(vector.create(0, 0, 0))

local x_vector = vector.create(1, 0, 0)
local y_vector = vector.create(0, 1, 0)
local z_vector = vector.create(0, 0, 1)

local position = vector.zero
local velocity = vector.zero

local pitch = 0
local yaw = 0

const FLOOR = 0
const GRAVITY = 9.8
const FRICTION = 2

local function look_to_euler(look: vector): vector
	look = vector.normalize(look)

	local x = math.asin(look.x)
	local y = math.atan2(-look.x, look.z)
	local z = 0

	return vector.create(x, y, z)
end

local function fuzzyeq(a: number,b: number,epsilon: number) : boolean
	return math.abs(a - b) <= epsilon
end

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

	local direction = vector.zero

	if key_map[enum.keycode.W] then
		direction -= z_vector
	end

	if key_map[enum.keycode.S] then
		direction += z_vector
	end

	if key_map[enum.keycode.A] then
		direction -= x_vector
	end

	if key_map[enum.keycode.D] then
		direction += x_vector
	end

	if key_map[enum.keycode.SPACE] and position.y <= FLOOR then
		velocity += vector.create(0, 5,0)
	end

	local acceleration = direction - (FRICTION * velocity)
	if position.y > FLOOR then
		acceleration += vector.create(0, -GRAVITY,0)
	end

	velocity += acceleration * dt
	position += velocity * speed * dt

	position = vector.create(position.x,math.max(position.y,0),position.z)

	a:setPosition(position)

	camera_offset = vector.create(
		camera_distance * math.sin(yaw) * math.cos(pitch), 
		1.5 + (camera_distance * math.sin(pitch)), 
		camera_distance * math.cos(yaw) * math.cos(pitch)
	)

	camera.lookAt(position + camera_offset,position)
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

	pitch += deltaY * 0.003
	yaw -=  deltaX * 0.003

	pitch = math.clamp(pitch, -math.rad(45), math.rad(45))
end)
