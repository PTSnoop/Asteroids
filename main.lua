res_x = love.graphics.getWidth()
res_y = love.graphics.getHeight()

bullet = {
    alive = true,
    theta = 0,
    x = 0,
    y = 0,
    vx = 0,
    vy = 0
}

roid = {
    alive = true,
    theta = 0,
    x = 0,
    y = 0,
    vx = 0,
    vy = 0
}

function collis(ax,ay,bx,by,val)
    return (ax-bx)^2 + (ay-by)^2 < val^2
end

function bullet.update(self,dt)
    self.x = self.x + self.vx*dt
    self.y = self.y + self.vy*dt
    if self.x < -30 then self.alive = false end
    if self.x > res_x+30 then self.alive = false end
    if self.y < -30 then self.alive = false end
    if self.y > res_y+30 then self.alive = false end
end

function roid.update(self,dt)
    self.x = self.x + self.vx*dt
    self.y = self.y + self.vy*dt

    if self.x < -30 then self.x = res_x+30 end
    if self.x > res_x+30 then self.x = -30 end
    if self.y < -30 then self.y = res_y+30 end
    if self.y > res_y+30 then self.y = -30 end
end

function newbullet()
    a = {}
    setmetatable(a, {__index = bullet})
    return a
end

function newroid()
    a = {}
    setmetatable(a, {__index = roid})
    return a
end

function spawn_a_roid()
    a_roid = newroid()
    looping = true
    while looping do
        a_roid.x = math.random()*(res_x+30)-15
        a_roid.y = math.random()*(res_y+30)-15
        if a_roid.x > -10 and a_roid.x < res_x
        and a_roid.y > -10 and a_roid.y < res_y
            then looping = true
            else looping = false
        end
    end
    a_roid.theta = math.random()*2*math.pi
    a_roid.vx = math.cos(a_roid.theta)*20*(1+roidrate*1000)
    a_roid.vy = math.sin(a_roid.theta)*20*(1+roidrate*1000)
    return a_roid
end

function love.load()
    ship = love.graphics.newImage("spacechevron.png")
    aroid = love.graphics.newImage("asteroid.png")
    shard = love.graphics.newImage("shard.png")
    blob = love.graphics.newImage("blob.png")
    gameover = love.graphics.newImage("gameover.png")
	font = love.graphics.newFont(love._vera_ttf, 14)
    love.graphics.setFont(font)

    p = love.graphics.newParticleSystem(blob,1000)
    p:setEmissionRate(100)
    p:setSpeed(100,200)
    p:setSize(0.2,0.5)
    p:setColor(255,255,255,255,255,255,255,255)
    p:setPosition(50,100)
    p:setRadialAcceleration(-35,-35)
    p:setLifetime(0.2)
    p:setParticleLife(0.5)
    p:setDirection(0)
    p:setSpread(360)
    p:setTangentialAcceleration(0)
    p:stop()

    q = love.graphics.newParticleSystem(shard,1000)
    q:setEmissionRate(100)
    q:setSpeed(100,200)
    q:setSize(0.2,0.5)
    q:setColor(255,255,255,255,255,255,255,255)
    q:setPosition(50,100)
    q:setRotation(0,720)
    q:setRadialAcceleration(-35,-35)
    q:setLifetime(0.2)
    q:setParticleLife(0.5)
    q:setDirection(0)
    q:setSpread(360)
    q:setTangentialAcceleration(0)
    q:stop()


	--[[
    p = love.graphics.newParticleSystem(ship, 1000)
	p:setEmissionRate(100)
	p:setSpeed(300, 400)
	p:setGravity(0)
	p:setSize(2, 1)
	p:setColor(255, 255, 255, 255, 58, 128, 255, 0)
	p:setPosition(400, 300)
	p:setLifetime(1)
	p:setParticleLife(1)
	p:setDirection(0)
	p:setSpread(360)
	p:setRadialAcceleration(-2000)
	p:setTangentialAcceleration(1000)
	p:setPosition(500,300)
    p:start()
	--p:stop()
    ]]--
    restart()
end

function restart()
    theta = 0
    vx = 0
    vy = 0
    x = 300
    y = 300
    alive = true
    score = 0

    flyspeed = 100
    rotspeed = 2

    bulletstorage = {}
    bullettime = 0

    roidrate = 0.0001
    roidstorage = {}
    for i = 1,50 do
        table.insert(roidstorage,spawn_a_roid())
    end


end

function love.update(dt)
    roidrate = roidrate + 0.0001*dt
    if love.keyboard.isDown("a") or love.keyboard.isDown("left") then theta = theta - rotspeed*dt end
    if love.keyboard.isDown("d") or love.keyboard.isDown("right") then theta = theta + rotspeed*dt end

    if love.keyboard.isDown("w") or love.keyboard.isDown("up") then 
        vx = vx + math.cos(theta)*flyspeed*dt
        vy = vy + math.sin(theta)*flyspeed*dt
    end

    if not alive and love.keyboard.isDown("return") then
        restart()
    end

    if love.keyboard.isDown(" ") and alive then
        if bullettime > 0.5 then
            bullettime = 0
            a = newbullet()
            a.x = x
            a.y = y
            a.vx = vx
            a.vy = vy
            a.theta = theta
            a.vx = a.vx + math.cos(a.theta)*400
            a.vy = a.vy + math.sin(a.theta)*400
            table.insert(bulletstorage,a)
        end
    end
    bullettime = bullettime + dt

    for i=1,#roidstorage do
        roidstorage[i].update(roidstorage[i],dt)
    end
    for i=1,#bulletstorage do
        if bulletstorage[i] then
            bulletstorage[i].update(bulletstorage[i],dt)
            if not bulletstorage[i].alive then
                table.remove(bulletstorage,i) 
            end
        end
    end

    if alive then
    for i=1,#roidstorage do
        for j=1,#bulletstorage do
            if roidstorage[i] and bulletstorage[j] then
            if collis(roidstorage[i].x+64,roidstorage[i].y+64,bulletstorage[j].x+64,bulletstorage[j].y+64,35) then
                roidstorage[i].alive = false
                p:setPosition(roidstorage[i].x,roidstorage[i].y)
                p:start()
                table.remove(roidstorage,i)
                table.remove(bulletstorage,j)
                score = score + 10
            end
            end
        end
    end
    end

    toppt_real_x, toppt_real_y = x+23*math.sin(-theta+(math.pi/2)),y+23*math.cos(-theta+(math.pi/2))

    sidept1_real_x, sidept1_real_y = x+30*math.sin(-theta+(3*math.pi/2) + (math.pi/6)),y+30*math.cos(-theta+(3*math.pi/2)+(math.pi/6))

    sidept2_real_x, sidept2_real_y = x+30*math.sin(-theta+(3*math.pi/2) - (math.pi/6)),y+30*math.cos(-theta+(3*math.pi/2)-(math.pi/6))


    ctrpt_real_x, ctrpt_real_y = x+10*math.sin(-theta+(3*math.pi/2)),y+10*math.cos(-theta+(3*math.pi/2))

    colliding = false
    for i=1,#roidstorage do
        if collis(roidstorage[i].x, roidstorage[i].y,toppt_real_x,toppt_real_y, 40) then colliding = true end
        if collis(roidstorage[i].x, roidstorage[i].y,sidept1_real_x,sidept1_real_y, 40) then colliding = true end
        if collis(roidstorage[i].x, roidstorage[i].y,sidept2_real_x,sidept2_real_y, 40) then colliding = true end
        if collis(roidstorage[i].x, roidstorage[i].y,ctrpt_real_x,ctrpt_real_y, 50) then colliding = true end
    end

    if colliding and alive then
        q:setPosition(x,y)
        q:start()
        alive = false
    end
        
    x = x + vx*dt
    y = y + vy*dt

    if x < -30 then x = res_x+30 end
    if x > res_x+30 then x = -30 end
    if y < -30 then y = res_y+30 end
    if y > res_y+30 then y = -30 end

    if vx > 1000 then vx = 1000 end
    if vy > 1000 then vy = 1000 end
    if vx < -1000 then vx = -1000 end
    if vy < -1000 then vy = -1000 end

    --p:setPosition(200,200)
    p:update(dt)
    q:update(dt)

    if math.random() < roidrate then
        table.insert(roidstorage,spawn_a_roid())
    end
end

function love.draw()
    if alive then
    love.graphics.draw(ship, x,y,theta+math.pi/2,0.5,0.5,64,64)
    end
    for i=1,#bulletstorage do
        love.graphics.rectangle("fill",bulletstorage[i].x, bulletstorage[i].y,5,5)
    end
    for i=1,#roidstorage do
        if roidstorage[i].alive then
            love.graphics.draw(aroid, roidstorage[i].x,roidstorage[i].y,roidstorage[i].theta+math.pi/2,0.5,0.5,64,64)
        end
    end
--[[
    love.graphics.setColor(255,0,0,255)
    love.graphics.circle("fill",toppt_real_x,toppt_real_y,5)
    love.graphics.circle("fill",sidept1_real_x,sidept1_real_y,5)
    love.graphics.circle("fill",sidept2_real_x,sidept2_real_y,5)
    love.graphics.circle("fill",ctrpt_real_x,ctrpt_real_y,15)
    love.graphics.circle("fill",x,y,5)
    love.graphics.setColor(255,255,255,255)
]]--
    love.graphics.draw(p,0,0)
    love.graphics.draw(q,0,0)

    if not alive then
    love.graphics.draw(gameover, 400,300)
    end

    love.graphics.setColor(0,0,0,255)
    love.graphics.rectangle("fill",res_x-50,28,40,24)
    love.graphics.setColor(255,255,255,255)
    love.graphics.printf(score,res_x-48,30,36,"right")
        
end


function love.keypressed(key)
if key == 'escape' then love.event.push('q') end
	
end
