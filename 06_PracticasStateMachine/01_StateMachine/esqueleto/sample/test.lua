MOAISim.openWindow("game", 1024, 768)

viewport = MOAIViewport.new()
viewport:setSize (1024, 768)
viewport:setScale (1024, -768)

layer = MOAILayer2D.new()
layer:setViewport(viewport)
MOAISim.pushRenderPass(layer)

texture_name = "dragon.png"
gfxQuad = MOAIGfxQuad2D.new()
gfxQuad:setTexture(texture_name)
char_size = 64
gfxQuad:setRect(-char_size/2, -char_size/2, char_size/2, char_size/2)
gfxQuad:setUVRect(0, 0, 1, 1)
  
prop = MOAIProp2D.new()
prop:setDeck(gfxQuad)
enemyProp = MOAIProp2D.new()
enemyProp:setDeck(gfxQuad)

entity = Character.new()
enemy = Character.new()
entity:setTarget(enemy)

-- Add prop to be the renderable for this character
entity:setProp(prop, layer)
enemy:setProp(enemyProp, layer)
-- Start the character (allow calls to OnUpdate)
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