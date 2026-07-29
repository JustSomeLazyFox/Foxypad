profile({
  name = "AsusNumberPad",
  regions = {
    region({
      shape = rect({ position = vec2(3270, 250), dimensions = vec2(600, 400), pivote = TOP_LEFT_CORNER --[[ optional ]] }),
      holdDuration = 1000,
      action = useProfile("touchpad") -- reserved profile name, cannot be custom defined with the same name
    }),
    region({
      shape = rect({ position = vec2(..., ...), dimensions = vec2(..., ...) }),
      -- since holdDuration nyot defined, there will be nyo holding requirement
      action = keyPress(KEY_KP7 --[[ integer constant, passed to the script ]])
    }),
    region({
      shape = circle({ position = vec2(..., ...), radius = ... }),
      performAfterRelease = true, -- action will perform after the touch is released (and only if the location of the finger is still in the region after release, false by default)
      action = function()
        execute("kitty")          -- ability to launch custom commands
      end
    })
  }
})
