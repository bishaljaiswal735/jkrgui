Com = {}
ComTable = {}

local com_i = 0
Com.NewComponent = function()
    com_i = com_i + 1
end

Com.Events = function()
    for _, com in ipairs(ComTable) do
        com:Event()
    end
end

Com.Draws = function()
    for _, com in ipairs(ComTable) do
        if com.SetScissor then com:SetScissor() end
        com:Draw()
        if com.SetScissor then Jkr.reset_scissor() end
    end
end

Com.AreaObject = {
    mIds = vec2(0, 0),
    mPosition_3f = vec3(0, 0, 0),
    mDimension_3f = vec3(0, 0, 0),
    New = function(self, inPosition_3f, inDimension_3f)
        local Obj = {
            mIds = vec2(0, 0), 
            mPosition_3f = vec3(0, 0, 0),
            mDimension_3f = vec3(0, 0, 0)
        }
        print("AreaObject Construction")
        setmetatable(Obj, self)
        self.__index = self
        Obj.mPosition_3f = inPosition_3f
        Obj.mDimension_3f = inDimension_3f

        local ShadowPos = vec3(inPosition_3f.x + 3, inPosition_3f.y + 3, inPosition_3f.z)
        local OutlinePos = vec3(inPosition_3f.x, inPosition_3f.y, inPosition_3f.z - 1)
        local AreaPos = vec3(inPosition_3f.x + 1, inPosition_3f.y + 1, inPosition_3f.z - 2)
        local AreaDimen = vec3(inDimension_3f.x - 2, inDimension_3f.y - 2, inDimension_3f.z)

        Com.NewComponent()
        ComTable[com_i] = Jkr.Components.Static.ShapeObject:New(ShadowPos, inDimension_3f, nil, nil)
        ComTable[com_i].mFillColor = Theme.Colors.Shadow
        Obj.mIds.x = com_i

        Com.NewComponent()
        ComTable[com_i] = Jkr.Components.Static.ShapeObject:New(OutlinePos, inDimension_3f, nil, nil)

        Com.NewComponent()
        ComTable[com_i] = Jkr.Components.Static.ShapeObject:New(AreaPos, AreaDimen, nil, nil)
        ComTable[com_i].mFillColor = vec4(1, 1, 1, 1)
        ComTable[com_i].mComponentObject.mFocusOnHover_b = false
        Obj.mIds.y = com_i
        print("No Of Components", com_i)
        print("AreaObject Construction Finished")
        return Obj
    end,
    Update = function(self, inPosition_3f, inDimension_3f)
        self.mPosition_3f = inPosition_3f
        self.mDimension_3f = inDimension_3f
        local i = self.mIds.x
        local ShadowPos = vec3(inPosition_3f.x + 3, inPosition_3f.y + 3, inPosition_3f.z)
        local OutlinePos = vec3(inPosition_3f.x, inPosition_3f.y, inPosition_3f.z - 1)
        local AreaPos = vec3(inPosition_3f.x + 1, inPosition_3f.y + 1, inPosition_3f.z - 2)
        local AreaDimen = vec3(inDimension_3f.x - 2, inDimension_3f.y - 2, inDimension_3f.z)
        ComTable[i]:Update(ShadowPos, inDimension_3f)
        ComTable[i + 1]:Update(OutlinePos, inDimension_3f)
        ComTable[i + 2]:Update(AreaPos, AreaDimen)
    end,
    Event = function(self)
        local i = self.mIds.x
        local mousePos = E.get_relative_mouse_pos()
        local isFocused = ComTable[i + 2].mComponentObject.mFocus_b -- TopMost Area
        if isFocused then
            local new_pos = vec3(self.mPosition_3f.x + mousePos.x, self.mPosition_3f.y + mousePos.y, self.mPosition_3f.z)
            self:Update(new_pos, self.mDimension_3f)
        end
    end
}

Com.TextLabelObject = {
    mIds = vec2(0, 0),
    mPositionToParent_3f = vec3(0, 0, 0),
    mPosition_3f = vec3(0, 0, 0),
    mDimension_3f = vec3(0, 0, 0),
    New = function(self, inText, inPosition_3f, inFontObject, inParent)
        print("TextLabelObject Construction")
        local Obj = {
            mIds = vec2(0, 0),
            mPosition_3f = vec3(0, 0, 0),
            mPositionToParent_3f = vec3(0, 0, 0),
            mDimension_3f = vec3(0, 0, 0)
        }
        setmetatable(Obj, self)
        self.__index = self
        Obj.mPosition_3f = inPosition_3f
        Obj.mPositionToParent_3f = inPosition_3f
        Com.NewComponent()
        Obj.mIds.x = com_i
        if inParent then
            Obj.SetParent(inParent)
        end
        ComTable[com_i] = Jkr.Components.Static.TextObject:New(inText, inPosition_3f, inFontObject)
        print("TextLabelObject Construction Finished")
        return Obj
    end,
    SetParent = function(self, inObject)
        local pos = vec3(inObject.mPosition_3f.x + self.mPositionToParent_3f.x, inObject.mPosition_3f.y + self.mPositionToParent_3f.y, self.mPosition_3f.z)
        self:Update(pos)
        if inObject.mPosition_3f.x > 0 and inObject.mPosition_3f.y > 0 then
             ComTable[self.mIds.x].mScissorPosition_2f = vec2(inObject.mPosition_3f.x, inObject.mPosition_3f.y)
             ComTable[self.mIds.x].mScissorDimension_2f = vec2(inObject.mDimension_3f.x, inObject.mDimension_3f.y)
        end
    end,
    Update = function (self, inPosition_3f)
        self.mPosition_3f = inPosition_3f
        ComTable[self.mIds.x]:Update(inPosition_3f)
    end
}



Com.TextButtonObject = {
    mPositionToParent_3f = vec3(0, 0, 0),
    mPadding = 5,
    mTextObject = nil,
    New = function (self, inText, inFontObject, inPosition_3f, inDimension_3f, inParent)
        local Obj = Com.AreaObject:New(inPosition_3f, inDimension_3f)
        setmetatable(self, Com.AreaObject) -- Inherits Com.AreaObject
        setmetatable(Obj, self)
        self.__index = self

        Obj.mTextObject = {}
        Obj.mPositionToParent_3f = {}
        Obj.mPadding = {}
        Obj.mPadding = 5
        Obj.mPositionToParent_3f = inPosition_3f
        local Position = vec3(inPosition_3f.x + Obj.mPadding, inPosition_3f.y + inDimension_3f.y - Obj.mPadding, inPosition_3f.z - 3)
        Obj.mTextObject = Com.TextLabelObject:New(inText, Position, inFontObject)
        if inParent then
            Obj.SetParent(inParent)
        end
        return Obj
    end,
    Event = function(self)
        
    end,
    SetParent = function(self, inObject)
        local pos = vec3(inObject.mPosition_3f.x + self.mPositionToParent_3f.x, inObject.mPosition_3f.y + self.mPositionToParent_3f.y, self.mPosition_3f.z)
        self:Update(pos, self.mDimension_3f)
        self.mTextObject:SetParent(inObject)
    end
}