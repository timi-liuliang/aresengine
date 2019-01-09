-- 全局变量
m_dir = 1;

-- 初始化
function Init()
	this:SetResPath("data\\m1\\General\\Bull.m1");

	this:SetPosition( 10, 0, 0);
end

-- 循环
function Loop( elapsedTime)
	local pos = this:GetPosition();
	if( pos.x>3) then
		m_dir = -1;
	end
	if( pos.x<0) then
		m_dir = 1;
	end

    pos.x = pos.x + m_dir * elapsedTime;

	this:SetPosition( pos);
end