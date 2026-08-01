activation({
	shape = rect({ position = vec2(3270, 250), dimensions = vec2(600, 400) }),
	holdDuration = 1000,
	idleTimeoutSeconds = -1,
	soundWhenEnabled = "~/.config/foxypad/sounds/bell.mp3",
})

local function getStandardKeyRect(pos)
	return rect({
		position = pos,
		dimensions = vec2(631, 467),
	})
end

profile({
	name = "Numberpad",
	regions = {
		--[[
		=================================
    -------------NUMBERS-------------
    =================================
    --]]
		region({
			shape = rect({ position = vec2(315, 311), dimensions = vec2(694, 467) }),
			action = keyPress(KEY_KP7),
		}),
		region({
			--shape = rect({ position = vec2(1041, 311), dimensions = vec2(631, 467) }),
			shape = getStandardKeyRect(vec2(1041, 311)),
			action = keyPress(KEY_KP8),
		}),
		region({
			shape = getStandardKeyRect(vec2(1735, 311)),
			action = keyPress(KEY_KP9),
		}),
		region({
			shape = rect({ position = vec2(315, 840), dimensions = vec2(694, 467) }),
			action = keyPress(KEY_KP4),
		}),
		region({
			shape = getStandardKeyRect(vec2(1041, 840)),
			action = keyPress(KEY_KP5),
		}),
		region({
			shape = getStandardKeyRect(vec2(1735, 840)),
			action = keyPress(KEY_KP6),
		}),
		region({
			shape = rect({ position = vec2(315, 1463), dimensions = vec2(694, 467) }),
			action = keyPress(KEY_KP1),
		}),
		region({
			shape = getStandardKeyRect(vec2(1041, 1463)),
			action = keyPress(KEY_KP2),
		}),
		region({
			shape = getStandardKeyRect(vec2(1735, 1463)),
			action = keyPress(KEY_KP3),
		}),
		region({
			shape = rect({ position = vec2(315, 2023), dimensions = vec2(1356, 467) }),
			action = keyPress(KEY_KP0),
		}),

		--[[
    ==================================
    -------------SYMBOLS--------------
    ==================================
    --]]
		region({
			shape = getStandardKeyRect(vec2(1735, 2023)),
			action = keyPress(KEY_KPDOT),
		}),
		region({
			shape = getStandardKeyRect(vec2(2398, 311)),
			action = keyPress(KEY_KPSLASH),
		}),
		region({
			shape = getStandardKeyRect(vec2(2398, 840)),
			action = keyPress(KEY_KPASTERISK),
		}),
		region({
			shape = getStandardKeyRect(vec2(2398, 1463)),
			action = keyPress(KEY_KPMINUS),
		}),
		region({
			shape = getStandardKeyRect(vec2(2398, 2023)),
			action = keyPress(KEY_KPPLUS),
		}),

		--[[
    ==============================================
    -------------------SPECIAL--------------------
    ==============================================
    --]]
		region({
			shape = getStandardKeyRect(vec2(3092, 840)),
			action = keyPress(KEY_BACKSPACE),
		}),
		region({
			shape = rect({ position = vec2(3092, 1463), dimensions = vec2(694, 996) }),
			action = keyPress(KEY_KPENTER),
		}),
	},
})
