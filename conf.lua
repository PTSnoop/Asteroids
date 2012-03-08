
function love.conf(t)
    t.title             = "Asteroids"
    t.identity          = "Roids"
    t.console           = false
    t.screen.width      = 1024
    t.screen.height     = 600
    t.screen.fullscreen = false
    t.screen.vsync      = true
    t.modules.joystick  = true
    t.modules.audio     = true
    t.modules.keyboard  = true
    t.modules.event     = true
    t.modules.image     = true
    t.modules.graphics  = true
    t.modules.timer     = true
    t.modules.mouse     = true
    t.modules.sound     = true
    t.modules.physics   = false
    t.modules.thread    = true
end
