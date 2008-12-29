x = -0.5
y = -0.5

function pulse()
	draw_rectangle(x, y, 50, 200)

	if up_pressed() then y = y + 0.01 end
	if down_pressed() then y = y - 0.01 end
	if right_pressed() then x = x + 0.01 end
	if left_pressed() then x = x - 0.01 end
end

