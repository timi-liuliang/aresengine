-- 游戏开始
function Init()
	-- 初始化英雄
	this:AddHero("LuoJie.lua");
	this:AddHero("BaiChuan.lua");

	-- 开始战斗
	this:StartBattle( "data\\maps\\Bridge\\Bridge.map", 0, 1);
end

-- 逻辑循环
function Loop()

end

-- 结束
function End()

end