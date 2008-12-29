Rect = {x = 0, y = 0, width = 0.3, height = 0.3}

function Rect:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function Rect:draw()
	draw_rectangle(self.x, self.y, self.width, self.height)
end

function Rect:move()
	if up_pressed()    then self.y = self.y + 0.01 end
	if down_pressed()  then self.y = self.y - 0.01 end
	if right_pressed() then self.x = self.x + 0.01 end
	if left_pressed()  then self.x = self.x - 0.01 end
end

rect = Rect:new{x = -0.5, y = -0.5, width=0.2, height=0.5}

function pulse()
	rect:move()
	rect:draw()
end

