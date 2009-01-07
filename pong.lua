math.randomseed(os.time())

Rect = {x = 0, y = 0, width = 0.3, height = 0.3, red = 0.0, green = 1.0, blue = 0.0}

function Rect:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function Rect:draw()
	draw_rectangle(self.x, self.y, self.width, self.height, self.red, self.green, self.blue)
end

function Rect:intersects(other)
	if math.abs(self.x - other.x) < (self.width + other.width) and
	   math.abs(self.y - other.y) < (self.height + other.height) then
		return true
	else
		return false
	end
end

---------------------------

Paddle = {y = 0, side = 1}

function Paddle:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	o:init()
	return o
end

function Paddle:init()
	self.rect = Rect:new{y = y, width = 0.05, height = 0.3}

	if self.side == 1 then
		self.rect.x = -0.9
		self.rect.green = 1.0
	else
		self.rect.x = 0.9
		self.rect.green = 0.0
		self.rect.blue = 1.0
	end
end

function Paddle:draw()
	self.rect.y = self.y
	self.rect:draw()
end

function Paddle:move()
	if self.side == 1 then
		if up_pressed()    then self.y = self.y + 0.01 end
		if down_pressed()  then self.y = self.y - 0.01 end
	else
		if self.y < ball.y then self.y = self.y + 0.01 end
		if self.y > ball.y then self.y = self.y - 0.01 end
	end
end

---------------------------

Ball = {x = 0, y = 0, vx = 0.0, vy = 0.0}

function Ball:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	o:init()
	return o
end

function Ball:init()
	self.rect = Rect:new{x = x, y = y, width = 0.03, height = 0.03, red = 1.0, green = 0.0, blue = 0.0}
end

function Ball:move()
	newrect = Rect:new(self.rect)
	newrect.x = newrect.x + self.vx
	newrect.y = newrect.y + self.vy

	for _, paddle in pairs(paddles) do
		if paddle.rect:intersects(newrect) then
			self.vx = self.vx * (-1.0 + math.random(-5, 5) * 0.1)
			self.vy = self.vy * (-1.0 + math.random(-5, 5) * 0.1)
		end
	end

	self.x = self.x + self.vx
	self.y = self.y + self.vy
end

function Ball:draw()
	self.rect.x = self.x
	self.rect.y = self.y
	self.rect:draw()
end

---------------------------

function describe(class, func)
	print(class)
	func()
	print()
end

function it(descr, func)
	if (func()) then
		print("- ", descr)
	else
		print("[fail] ", descr)
	end
end

function all_tests()
	describe("Rect", function()
		r1 = Rect:new{x = 0, y = 0, w = 3, h = 1}
		r2 = Rect:new{x = 1, y = 0, w = 2, h = 2}
		r3 = Rect:new{x = 4, y = 0, w = 2, h = 2}

		it("intersects two rectangles", function()
			return r1:intersects(r2) == true
		end)

		it("doesn't intersect non-intersecting rectangles", function()
			return r1:intersects(r3) == false
		end)
	end)
end

---------------------------

paddles = { Paddle:new{side=1}, Paddle:new{side=2} }
ball = Ball:new{x = 0.0, y = 0.0, vx = -0.005, vy = 0.005}

function pulse()
	for _, paddle in pairs(paddles) do
		paddle:move()
		paddle:draw()
	end

	ball:move()
	ball:draw()
end

