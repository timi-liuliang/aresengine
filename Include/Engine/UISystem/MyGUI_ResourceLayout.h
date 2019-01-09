/*!
	@file
	@author		George Evmenov
	@date		06/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_RESOURCE_LAYOUT_H__
#define __MYGUI_RESOURCE_LAYOUT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_IResource.h"

namespace MyGUI
{
	// 控制器信息
	struct MYGUI_EXPORT ControllerInfo
	{
		std::string type;
		MapString properties;
	};

	// 界面信息
	struct MYGUI_EXPORT WidgetInfo
	{
		enum PositionType 
		{ 
			Pixels, 
			Relative, 
			None 
		};

		std::vector<WidgetInfo>		childWidgetsInfo;			// not map because there might be several properties with same name (for example List items)
		VectorStringPairs			properties;
		MapString					userStrings;
		std::vector<ControllerInfo> controllers;
		std::string					type;
		std::string					skin;
		Align						align;
		WidgetStyle					style;
		std::string					name;
		std::string					layer;

		PositionType				positionType;
		IntCoord					intCoord;
		FloatCoord					floatCoord;

		// 构造函数
		WidgetInfo()
			: align(Align::Default), style(WidgetStyle::Child), positionType(None)
		  {}
	};
	typedef std::vector<WidgetInfo> VectorWidgetInfo;

	//-----------------------------------------------
	// ResourceLayout 2012-10-18 
	//-----------------------------------------------
	class MYGUI_EXPORT ResourceLayout : public IResource
	{
		MYGUI_RTTI_DERIVED( ResourceLayout )

	public:
		ResourceLayout();

		ResourceLayout(xml::ElementPtr _node, const std::string& _file);

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		const VectorWidgetInfo& getLayoutData() const;

		/** Create widgets described in layout */
		VectorWidgetPtr createLayout(const std::string& _prefix = "", Widget* _parent = nullptr);

		// widget info -> Widget
		Widget* createWidget(const WidgetInfo& _widgetInfo, const std::string& _prefix = "", Widget* _parent = nullptr, bool _template = false);

	protected:
		// xml -> widget info
		WidgetInfo parseWidget(xml::ElementEnumerator& _widget);

	protected:
		VectorWidgetInfo mLayoutData;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_LAYOUT_H__
