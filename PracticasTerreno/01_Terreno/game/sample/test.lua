MOAISim.openWindow("game", 1024, 768)

viewport = MOAIViewport.new()
viewport:setSize (1024, 768)
viewport:setScale (1024, -768)

layer = MOAILayer2D.new()
layer:setViewport(viewport)
MOAISim.pushRenderPass(layer)

function drawBackground(image, sizeX, sizeY)
    gfxQuad = MOAIGfxQuad2D.new()
    gfxQuad:setTexture(image)
    gfxQuad:setRect(-sizeX/2, -sizeY/2, sizeX/2, sizeY/2)
    gfxQuad:setUVRect(0, 0, 1, 1)
    prop = MOAIProp2D.new()
    prop:setDeck(gfxQuad)
    prop:setLoc(posX, posY)
    layer:insertProp(prop)
end

drawBackground("background.png", 1024, 768)

texture_name = "ghost32.png"
gfxQuad = MOAIGfxQuad2D.new()
gfxQuad:setTexture(texture_name)
gfxQuad:setRect(-16, -16, 16, 16)
gfxQuad:setUVRect(0, 0, 1, 1)
  
prop = MOAIProp2D.new()
prop:setDeck(gfxQuad)

entity = Character.new()
-- Add prop to be the renderable for this character
entity:setProp(prop, layer)
-- Start the character (allow calls to OnUpdate)
entity:start()
entity:setLoc(0, 0)
entity:setRot(0)
entity:setLinearVel(10, 20)
entity:setAngularVel(30)

-- Enable Debug Draw
debug = MOAIDrawDebug.get();
layer:setDrawDebug(debug)
-- Add this character to draw debug
MOAIDrawDebug.insertEntity(entity)


pathfinder = Pathfinder.new()
pathfinder:setStartPosition(5, 10)
pathfinder:setEndPosition(20, 40)
MOAIDrawDebug.insertEntity(pathfinder)

mouseX = 0
mouseY = 0

function onClick(down)
  pathfinder:setStartPosition(mouseX, mouseY)
end

function onRightClick(down)
  pathfinder:setEndPosition(mouseX, mouseY)
end

function pointerCallback(x, y)
    mouseX, mouseY = layer:wndToWorld(x, y)
end

MOAIInputMgr.device.mouseLeft:setCallback(onClick)
MOAIInputMgr.device.mouseRight:setCallback(onRightClick)
MOAIInputMgr.device.pointer:setCallback(pointerCallback)

function onKeyPressed(key, down)
	if key == 32 then
		if down then
			print(tostring(key))
		else
			pathfinder:pathfindStep()
		end
	end
end

if (MOAIInputMgr.device.keyboard) then
    MOAIInputMgr.device.keyboard:setCallback(onKeyPressed)
end
