#pragma once

#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsPixmapItem>
#include <Core/Graphics/AresTexture.h>
#include "ui_TextureTool.h"
#include "TextureSetBuild.h"

using namespace Ares;

namespace QT_UI
{
	//------------------------------------
	// 纹理查看编辑器 2012-7-23 帝林
	//------------------------------------
	class TextureTool : public QMainWindow, public Ui_TextureTool
	{
		Q_OBJECT

	public:
		// 构造函数
		TextureTool( QWidget* parent=0);

		// 析构函数
		~TextureTool();

		// 初始化App
		void InitApp();

		// 显示纹理
		void ShowTexture(  TexturePtr tex);

	private slots:
		// 打开纹理
		void Open();

		// 纹理集添加子纹理
		void AddSubTex();

	private:
		QGraphicsScene			m_graphicsScene;		// 渲染场景
		QGraphicsPixmapItem		m_graphicsItem;			// 显示用
		TextureSetBuild*		m_textureSetBuild;		// 构建器
	};
}