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

---------------------------

Paddle = {y = 0, side = 1}

function Paddle:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self

	o.rect = Rect:new{y = y, width = 0.05, height = 0.3}

	if o.side == 1 then
		o.rect.x = -0.9
		o.rect.green = 1.0
	else
		o.rect.x = 0.9
		o.rect.green = 0.0
		o.rect.blue = 1.0
	end

	return o
end

function Paddle:draw()
	self.rect.y = self.y
	self.rect:draw()
end

function Paddle:move()
	if self.side == 1 then
		if up_pressed()    then self.y = self.y + 0.01 end
		if down_pressed()  then self.y = self.y - 0.01 end
	end
end

---------------------------

paddles = { Paddle:new{side=1}, Paddle:new{side=2} }

function pulse()
	for _, paddle in pairs(paddles) do
		paddle:move()
		paddle:draw()
	end
end

