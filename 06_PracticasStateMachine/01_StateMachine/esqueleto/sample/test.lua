

-- Functions

function GenerateQuad(texname)
    local size = 64
    local quad = MOAIGfxQuad2D.new()
    quad:setTexture(texname)
    quad:setRect(-size / 2, -size / 2, size / 2, size / 2)
    quad:setUVRect(0, 0, 1, 1)
    return quad
end

-- Script
MOAISim.openWindow("game", 1024, 768)

viewport = MOAIViewport.new()
viewport:setSize (1024, 768)
viewport:setScale (1024, -768)

layer = MOAILayer2D.new()
layer:setViewport(viewport)
MOAISim.pushRenderPass(layer)

idleImage = GenerateQuad("dragon_idle.png")
alarmImage = GenerateQuad("dragon_alarm.png")
attackImage = GenerateQuad("dragon_attack.png")
deadImage = GenerateQuad("dragon_dead.png")
hitImage = GenerateQuad("dragon_hit.png")
windupImage = GenerateQuad("dragon_windup.png")

  
prop = MOAIProp2D.new()
prop:setDeck(idleImage)
enemyProp = MOAIProp2D.new()
enemyProp:setDeck(idleImage)

-- Entity
entity = Character.new()
entity:addImage(idleImage)
entity:addImage(alarmImage)
entity:addImage(attackImage)
entity:addImage(deadImage)
entity:addImage(hitImage)
entity:addImage(windupImage)
-- Add prop to be the renderable 
entity:setProp(prop, layer)

-- Enemy
enemy = Character.new()
enemy:setProp(enemyProp, layer)

-- Start the character (allow calls to OnUpdate)
entity:setTarget(enemy)
entity:loadStateMachine()
entity:startStateMachine()
entity:start()
entity:setLoc(-50, -50)
entity:setRot(-135)
entity:setLinearVel(-20, -20)
--entity:setAngularVel(30)

-- Enable Debug Draw
debug = MOAIDrawDebug.get();
layer:setDrawDebug(debug)
-- Add this character to draw debug
MOAIDrawDebug.insertEntity(entity)

mouseX = 0
mouseY = 0

function onClick(down)
  --entity:setLoc(mouseX, mouseY)
  --entity:setRot(-135)
  enemy:setLoc(mouseX, mouseY)
end

function pointerCallback(x, y)
    mouseX, mouseY = layer:wndToWorld(x, y)
end

MOAIInputMgr.device.mouseLeft:setCallback(onClick)
MOAIInputMgr.device.pointer:setCallback(pointerCallback)