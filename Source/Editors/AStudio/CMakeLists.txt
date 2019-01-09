# 函数
MACRO(ADD_QTFORMS uiFile hFile cppFile group mocGroup)
	SET(QTUI_HEADER_UI)
	SET(UI_HEADERS_MOC)
	QT4_WRAP_UI(QTUI_HEADER_UI	${uiFile})
	QT4_WRAP_CPP(UI_HEADERS_MOC ${hFile})
	
	# UI 文件
	SOURCE_GROUP(${group}	 FILES ${hFile} ${cppFile} ${uiFile} ${QTUI_HEADER_UI})
	SOURCE_GROUP(${mocGroup} FILES ${UI_HEADERS_MOC})
	
	SET(ASTUDIO_QT_FILES ${ASTUDIO_QT_FILES} ${hFile} ${cppFile} ${uiFile} ${QTUI_HEADER_UI} ${UI_HEADERS_MOC})
ENDMACRO(ADD_QTFORMS)

#组文件
MACRO(ADD_GROUP2 file1 file2 groupName)
	# UI 文件
	SOURCE_GROUP(${groupName} FILES ${file1} ${file2})
	
	SET(ASTUDIO_GROUP_FILES  ${ASTUDIO_GROUP_FILES} ${file1} ${file2})
ENDMACRO(ADD_GROUP2)


# 库依赖
SET(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/External/Qt/4.8.1)
FIND_PACKAGE(Qt4 REQUIRED)

# 附加包含目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/Editors/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/Editors/QLibrary)

# 添加预处理器
IF(WIN32)
	ADD_DEFINITIONS(-DPLATFORM_WIN32)
	ADD_DEFINITIONS(-DPBRT_PROBES_NONE)
	ADD_DEFINITIONS(-DPBRT_HAS_OPENEXR)
	ADD_DEFINITIONS(-DZLIB_WINAPI)
ELSEIF(APPLE)
	IF(${CMAKE_OSX_SYSROOT} MATCHES "MacOSX.platform")
		ADD_DEFINITIONS(-DPLATFORM_MAC)
	ELSE()
		ADD_DEFINITIONS(-DPLATFORM_IOS)
	ENDIF()
ENDIF()

# 设置模块名称
SET(MODULE_NAME AStudio)

# 源文件路径
SET(MODULE_HEADER_PATH		${PROJECT_SOURCE_DIR}/Include/${MODULE_NAME})
SET(MODULE_SOURCE_PATH		${PROJECT_SOURCE_DIR}/Source/Editors/${MODULE_NAME})
SET(MODULE_QLIBRARY_PATH	${PROJECT_SOURCE_DIR}/Source/Editors/QLibrary)

# VS配置 http://www.oschina.net/code/explore/mariadb-5.2.4/CMakeLists.txt
IF(MSVC)
	# 禁用特定警告
	SET(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG} /wd4996 /wd4819 /MP")
    SET(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE} /wd4996 /wd4819 /MP")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /wd4996 /wd4819")
    SET(CMAKE_C_FLAGS_DEBUG				"${CMAKE_C_FLAGS_DEBUG} /wd4996 /wd4819")
    SET(CMAKE_C_FLAGS_RELEASE			"${CMAKE_C_FLAGS_RELEASE} /wd4996 /wd4819")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO	"${CMAKE_C_FLAGS_RELWITHDEBINFO} /wd4996 /wd4819")

	# MT MTD
	STRING(REPLACE "/MD"  "/MT"  CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
    STRING(REPLACE "/MD"  "/MT"  CMAKE_C_FLAGS_RELWITHDEBINFO   ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG            ${CMAKE_C_FLAGS_DEBUG})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG_INIT       ${CMAKE_C_FLAGS_DEBUG_INIT})
    STRING(REPLACE "/MD"  "/MT"  CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
    STRING(REPLACE "/MD"  "/MT"  CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG_INIT     ${CMAKE_CXX_FLAGS_DEBUG_INIT})
ENDIF(MSVC)

# 图标
SET(MODULE_ICON_FILE
	${MODULE_SOURCE_PATH}/App.rc	
)
SOURCE_GROUP("AStudio\\UI\\Generated Files\\Resources\\" FILES ${MODULE_ICON_FILE})

# QT资源
SET(MODULE_QT_RESOURCE_PATH ${MODULE_SOURCE_PATH}/App.qrc)
QT4_ADD_RESOURCES(MODULE_RCC_SRCS ${MODULE_QT_RESOURCE_PATH})

SOURCE_GROUP("AStudio\\UI\\Generated Files\\Resources\\" FILES ${MODULE_RCC_SRCS})
SOURCE_GROUP("AStudio\\UI\\Generated Files\\Resources\\" FILES ${MODULE_QT_RESOURCE_PATH})

# 包含源文件
SET(MODULE_LOGIC_AI_SOURCE_FILES
	${MODULE_SOURCE_PATH}/AILogic.h
	${MODULE_SOURCE_PATH}/AILogic.cpp
)

SET(MODULE_LOGIC_CAMERA_SOURCE_FILES
	${MODULE_SOURCE_PATH}/CameraLogic.h
	${MODULE_SOURCE_PATH}/CameraLogic.cpp
)
SET(MODULE_LOGIC_CMDLINE_SOURCE_FILES
	${MODULE_SOURCE_PATH}/CMDLine.h
	${MODULE_SOURCE_PATH}/CMDLine.cpp
)
SET(MODULE_LOGIC_CONFIG_SOURCE_FILES
	${MODULE_SOURCE_PATH}/ConfigSettings.h
	${MODULE_SOURCE_PATH}/ConfigMgr.h
	${MODULE_SOURCE_PATH}/ConfigMgr.cpp
)
SET(MODULE_LOGIC_ENTITYEDIT_SOURCE_FILES
	${MODULE_SOURCE_PATH}/EntityEditLogic.h
	${MODULE_SOURCE_PATH}/EntityEditLogic.cpp
)
SET(MODULE_LOGIC_ENTITYPUT_SOURCE_FILES
	${MODULE_SOURCE_PATH}/EntityPutLogic.h
	${MODULE_SOURCE_PATH}/EntityPutLogic.cpp
)
SET(MODULE_LOGIC_GPUBAKE_SOURCE_FILES
	${MODULE_SOURCE_PATH}/GpuLightMapBake.h
	${MODULE_SOURCE_PATH}/GpuLightMapBake.cpp
)
SET(MODULE_LOGIC_INPUTLISTENER_SOURCE_FILES
	${MODULE_SOURCE_PATH}/InputListener.h
	${MODULE_SOURCE_PATH}/InputListener.cpp
)
SET(MODULE_LOGIC_MOVELOGIC_SOURCE_FILES
	${MODULE_SOURCE_PATH}/DefaultProxy.h
	${MODULE_SOURCE_PATH}/DefaultProxy.cpp
	${MODULE_SOURCE_PATH}/CharacterProxy.h
	${MODULE_SOURCE_PATH}/CharacterProxy.cpp
	${MODULE_SOURCE_PATH}/MoveLogic.h
	${MODULE_SOURCE_PATH}/MoveLogic.cpp
)
SET(MODULE_LOGIC_NET_SOURCE_FILES
	${MODULE_SOURCE_PATH}/client.h
	${MODULE_SOURCE_PATH}/client.cpp
	${MODULE_SOURCE_PATH}/NetLogic.h
	${MODULE_SOURCE_PATH}/NetLogic.cpp
)
SET(MODULE_LOGIC_PHYSICS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/PhysicsLogic.h
	${MODULE_SOURCE_PATH}/PhysicsLogic.cpp
)
SET(MODULE_LOGIC_SETTINGS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/SettingsLogic.h
	${MODULE_SOURCE_PATH}/SettingsLogic.cpp
)
SET(MODULE_LOGIC_WATER_SOURCE_FILES
	${MODULE_SOURCE_PATH}/WaterEditLogic.h
	${MODULE_SOURCE_PATH}/WaterEditLogic.cpp
)
SET(MODULE_LOGIC_WORLDMAP_SOURCE_FILES
	${MODULE_SOURCE_PATH}/MapLogic.h
	${MODULE_SOURCE_PATH}/MapLogic.cpp
)
SET(MODULE_LOGIC_ENTRY_SOURCE_FILES
	${MODULE_SOURCE_PATH}/SCLogic.h
	${MODULE_SOURCE_PATH}/SCLogic.cpp
)
SET(LOGIC_ALL_SOURCE_FILES
	${MODULE_LOGIC_AI_SOURCE_FILES}
	${MODULE_LOGIC_CAMERA_SOURCE_FILES}
	${MODULE_LOGIC_CMDLINE_SOURCE_FILES}
	${MODULE_LOGIC_CONFIG_SOURCE_FILES}
	${MODULE_LOGIC_ENTITYEDIT_SOURCE_FILES}
	${MODULE_LOGIC_ENTITYPUT_SOURCE_FILES}
	${MODULE_LOGIC_GPUBAKE_SOURCE_FILES}
	${MODULE_LOGIC_INPUTLISTENER_SOURCE_FILES}
	${MODULE_LOGIC_MOVELOGIC_SOURCE_FILES}
	${MODULE_LOGIC_NET_SOURCE_FILES}
	${MODULE_LOGIC_PHYSICS_SOURCE_FILES}
	${MODULE_LOGIC_SETTINGS_SOURCE_FILES}
	${MODULE_LOGIC_WATER_SOURCE_FILES}
	${MODULE_LOGIC_WORLDMAP_SOURCE_FILES}
	${MODULE_LOGIC_ENTRY_SOURCE_FILES}
)

SOURCE_GROUP("SceneEdit\\Logic\\AI"				FILES ${MODULE_LOGIC_AI_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\Application"	FILES ${MODULE_LOGIC_APPLICATION_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\CameraLogic"	FILES ${MODULE_LOGIC_CAMERA_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\CMDLine"		FILES ${MODULE_LOGIC_CMDLINE_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\Config"			FILES ${MODULE_LOGIC_CONFIG_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\EntityEdit"		FILES ${MODULE_LOGIC_ENTITYEDIT_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\EntityPut"		FILES ${MODULE_LOGIC_ENTITYPUT_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\GpuBake"		FILES ${MODULE_LOGIC_GPUBAKE_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\InputListener"	FILES ${MODULE_LOGIC_INPUTLISTENER_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\MoveLogic"		FILES ${MODULE_LOGIC_MOVELOGIC_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\Net"			FILES ${MODULE_LOGIC_NET_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\Physics"		FILES ${MODULE_LOGIC_PHYSICS_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\Settings"		FILES ${MODULE_LOGIC_SETTINGS_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\Water"			FILES ${MODULE_LOGIC_WATER_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic\\WorldMap"		FILES ${MODULE_LOGIC_WORLDMAP_SOURCE_FILES})
SOURCE_GROUP("SceneEdit\\Logic"					FILES ${MODULE_LOGIC_ENTRY_SOURCE_FILES})


# AI_UI
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/AI.ui
	${MODULE_SOURCE_PATH}/AI_UI.h
	${MODULE_SOURCE_PATH}/AI_UI.cpp
	"SceneEdit\\\\UI\\\\AI_UI"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ContentBrowser
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ContentBrowser.ui
	${MODULE_SOURCE_PATH}/ContentBrowser.h
	${MODULE_SOURCE_PATH}/ContentBrowser.cpp
	"SceneEdit\\\\UI\\\\ContentBrowser"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#ImageProcess
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ImageProcess.ui
	${MODULE_SOURCE_PATH}/ImageProcess.h
	${MODULE_SOURCE_PATH}/ImageProcess.cpp
	"SceneEdit\\\\UI\\\\ImageProcess"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#Lightmap
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/LightMap.ui
	${MODULE_SOURCE_PATH}/LightMap.h
	${MODULE_SOURCE_PATH}/LightMap.cpp
	"SceneEdit\\\\UI\\\\Lightmap"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#SCMainWindow
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/SCMainWindow.ui
	${MODULE_SOURCE_PATH}/SCMainWindow.h
	${MODULE_SOURCE_PATH}/SCMainWindow.cpp
	"SceneEdit\\\\UI\\\\MainWindow"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#MapBuilder
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/MapBuilder.ui
	${MODULE_SOURCE_PATH}/MapBuilder.h
	${MODULE_SOURCE_PATH}/MapBuilder.cpp
	"SceneEdit\\\\UI\\\\MapBuilder"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#PropertyEdit
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/PropertyEditor.ui
	${MODULE_SOURCE_PATH}/PropertyEditor.h
	${MODULE_SOURCE_PATH}/PropertyEditor.cpp
	"SceneEdit\\\\UI\\\\PropertyEdit"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#FrameSceneInspector
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/SceneInspector.ui
	${MODULE_SOURCE_PATH}/SceneInspector.h
	${MODULE_SOURCE_PATH}/SceneInspector.cpp
	"SceneEdit\\\\UI\\\\SceneInspector"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

ADD_QTFORMS(
	""
	${MODULE_SOURCE_PATH}/SceneInspectorModel.h
	${MODULE_SOURCE_PATH}/SceneInspectorModel.cpp
	"SceneEdit\\\\UI\\\\SceneInspector"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# TerrainEdit
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/TerrainEdit.ui
	${MODULE_SOURCE_PATH}/TerrainEdit.h
	${MODULE_SOURCE_PATH}/TerrainEdit.cpp
	"SceneEdit\\\\UI\\\\PropertyEdit\\\\TerrainEdit"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# LightProperty
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/LightProperty.ui
	${MODULE_SOURCE_PATH}/LightProperty.h
	${MODULE_SOURCE_PATH}/LightProperty.cpp
	"SceneEdit\\\\UI\\\\PropertyEdit\\\\LightProperty"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# WorldMap
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/WorldMap.ui
	${MODULE_SOURCE_PATH}/WorldMap.h
	${MODULE_SOURCE_PATH}/WorldMap.cpp
	"SceneEdit\\\\UI\\\\WorldMap"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"		
)

# WaterEdit
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/WaterEdit.ui
	${MODULE_SOURCE_PATH}/WaterEdit.h
	${MODULE_SOURCE_PATH}/WaterEdit.cpp
	"SceneEdit\\\\UI\\\\WaterEdit"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"		
)

# VideoSetting
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/VideoSetting.ui
	${MODULE_SOURCE_PATH}/VideoSetting.h
	${MODULE_SOURCE_PATH}/VideoSetting.cpp
	"SceneEdit\\\\UI\\\\VideoSetting"
	"SceneEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# 加入当前的编译目录，因为新生成的头文件在这里
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# UI 文件
SET(MODULE_UI_LIBRARY
	${MODULE_QLIBRARY_PATH}/QDirectoryModel.h
	${MODULE_QLIBRARY_PATH}/QCheckBoxList.h
	${MODULE_QLIBRARY_PATH}/QColorSelect.h
	${MODULE_QLIBRARY_PATH}/QFileSelect.h
	${MODULE_QLIBRARY_PATH}/QProperty.hpp
	${MODULE_QLIBRARY_PATH}/QPropertyDelegate.h
	${MODULE_QLIBRARY_PATH}/QPropertyModel.h
	${MODULE_QLIBRARY_PATH}/QRadioImage.h
	${MODULE_QLIBRARY_PATH}/QMainWindowEx.h
	${MODULE_QLIBRARY_PATH}/QLuaSyntaxHighLighter.h
	${MODULE_QLIBRARY_PATH}/QShaderSyntaxHighLighter.h
	${MODULE_QLIBRARY_PATH}/QPbrtSyntaxHighLighter.h
	${MODULE_QLIBRARY_PATH}/QLoadTga.h
)
SET(MODULE_UI_CPP_LIBRARY
	${MODULE_QLIBRARY_PATH}/QDirectoryModel.cpp
	${MODULE_QLIBRARY_PATH}/QCheckBoxList.cpp
	${MODULE_QLIBRARY_PATH}/QColorSelect.cpp
	${MODULE_QLIBRARY_PATH}/QFileSelect.cpp
	${MODULE_QLIBRARY_PATH}/QPropertyDelegate.cpp
	${MODULE_QLIBRARY_PATH}/QPropertyModel.cpp
	${MODULE_QLIBRARY_PATH}/QRadioImage.cpp
	${MODULE_QLIBRARY_PATH}/QMainWindowEx.cpp
	${MODULE_QLIBRARY_PATH}/QLuaSyntaxHighLighter.cpp
	${MODULE_QLIBRARY_PATH}/QShaderSyntaxHighLighter.cpp
	${MODULE_QLIBRARY_PATH}/QPbrtSyntaxHighLighter.cpp
	${MODULE_QLIBRARY_PATH}/QLoadTga.cpp
)
SOURCE_GROUP("SceneEdit\\UI"			FILES ${MODULE_UI_FILES})
SOURCE_GROUP("SceneEdit\\UI"			FILES ${MODULE_UI_CPP_FILES})
SOURCE_GROUP("CMake Rules\\QLibrary"	FILES ${MODULE_UI_LIBRARY})
SOURCE_GROUP("CMake Rules\\QLibrary"	FILES ${MODULE_UI_CPP_LIBRARY})

QT4_WRAP_CPP(MODULE_UI_HEADERS_MOC ${MODULE_UI_FILES} ${MODULE_UI_LIBRARY})
SOURCE_GROUP("SceneEdit\\UI\\Generated Files\\Moc" FILES ${MODULE_UI_HEADERS_MOC})

#-----------------------Begin ModelEdit -----------------------#
# Logic
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOLogic.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOLogic.cpp
	"ModelEdit\\\\Logic"
)

# CameraLogic
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOCameraLogic.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOCameraLogic.cpp
	"ModelEdit\\\\Logic\\\\CameraLogic" 
)

# LogicHelper
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOLogicHelper.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOLogicHelper.cpp
	"ModelEdit\\\\Logic\\\\LogicHelper" 
)

# ModelEdit
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOModelEdit.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOModelEdit.cpp
	"ModelEdit\\\\Logic\\\\ModelEdit" 
)

# ConfigMgr
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOConfigMgr.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOConfigMgr.cpp
	"ModelEdit\\\\Logic\\\\ConfigMgr" 
)

# LightmapUVBuild
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOLightmapUVBuild.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOLightmapUVBuild.cpp
	"ModelEdit\\\\Logic\\\\LightmapUVBuild" 
)

# LightmapUVRender
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOLightmapUVRender.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOLightmapUVRender.cpp
	"ModelEdit\\\\Logic\\\\LightmapUVRender" 
)

# LightEditLogic
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOLightEditLogic.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOLightEditLogic.cpp
	"ModelEdit\\\\Logic\\\\LightEditLogic" 
)

# InputListener
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOInputListener.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOInputListener.cpp
	"ModelEdit\\\\Logic\\\\InputListener" 
)

# CollisionBuild
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOCollisionBuildLogic.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOCollisionBuildLogic.cpp
	"ModelEdit\\\\Logic\\\\CollisionBuild" 
)

# Transform
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOTransformLogic.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOTransformLogic.cpp
	"ModelEdit\\\\Logic\\\\Transform" 
)

# YNDFileFormat
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/MOYNDMeshToAres.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOYNDMeshToAres.cpp
	"ModelEdit\\\\Logic\\\\YNDFileFormat" 
)
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ModelEdit/YNDMesh/YNDMeshFile.h
	${MODULE_SOURCE_PATH}/ModelEdit/YNDMesh/YNDMeshFile.cpp
	"ModelEdit\\\\Logic\\\\YNDFileFormat" 
)

# MOMainWindow
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ModelEdit/MOMainWindow.ui
	${MODULE_SOURCE_PATH}/ModelEdit/MOMainWindow.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOMainWindow.cpp
	"ModelEdit\\\\UI\\\\MainWindow"
	"ModelEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# MOAnimationEdit
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ModelEdit/MOAnimationEdit.ui
	${MODULE_SOURCE_PATH}/ModelEdit/MOAnimationEdit.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOAnimationEdit.cpp
	"ModelEdit\\\\UI\\\\AnimationEdit"
	"ModelEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# MOUVBUild
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ModelEdit/MOUVBuild.ui
	${MODULE_SOURCE_PATH}/ModelEdit/MOUVBuild.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOUVBuild.cpp
	"ModelEdit\\\\UI\\\\UVBuild"
	"ModelEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# CollisionBuild
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ModelEdit/MOCollisionBuild.ui
	${MODULE_SOURCE_PATH}/ModelEdit/MOCollisionBuild.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOCollisionBuild.cpp
	"ModelEdit\\\\UI\\\\CollisionBuild"
	"ModelEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# MaterialView
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ModelEdit/MOMaterialView.ui
	${MODULE_SOURCE_PATH}/ModelEdit/MOMaterialView.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOMaterialView.cpp
	"ModelEdit\\\\UI\\\\MaterialView"
	"ModelEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ModelConverter
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ModelEdit/MOModelConverter.ui
	${MODULE_SOURCE_PATH}/ModelEdit/MOModelConverter.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOModelConverter.cpp
	"ModelEdit\\\\UI\\\\ModelConverter"
	"ModelEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# VideoSetting
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ModelEdit/MOVideoSetting.ui
	${MODULE_SOURCE_PATH}/ModelEdit/MOVideoSetting.h
	${MODULE_SOURCE_PATH}/ModelEdit/MOVideoSetting.cpp
	"ModelEdit\\\\UI\\\\VideoSetting"
	"ModelEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#-----------------------End   ModelEdit -----------------------#

#-----------------------Start FxEdit    -----------------------#
# FxLogic
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/FxEdit/FxLogic.h
	${MODULE_SOURCE_PATH}/FxEdit/FxLogic.cpp
	"FxEdit\\\\Logic" 
)

# FxEdit
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/FxEdit/FxEdit.h
	${MODULE_SOURCE_PATH}/FxEdit/FxEdit.cpp
	"FxEdit\\\\Logic\\\\FxEdit" 
)

# MainWindow
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/FxEdit/FxMainWindow.ui
	${MODULE_SOURCE_PATH}/FxEdit/FxMainWindow.h
	${MODULE_SOURCE_PATH}/FxEdit/FxMainWindow.cpp
	"FxEdit\\\\UI\\\\MainWindow"
	"FxEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ObjectViewer
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/FxEdit/FxObjectViewer.ui
	${MODULE_SOURCE_PATH}/FxEdit/FxObjectViewer.h
	${MODULE_SOURCE_PATH}/FxEdit/FxObjectViewer.cpp
	"FxEdit\\\\UI\\\\ObjectViewer"
	"FxEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# PropertyEdit
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/FxEdit/FxPropertyEdit.ui
	${MODULE_SOURCE_PATH}/FxEdit/FxPropertyEdit.h
	${MODULE_SOURCE_PATH}/FxEdit/FxPropertyEdit.cpp
	"FxEdit\\\\UI\\\\PropertyEdit"
	"FxEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#-----------------------End   FxEdit	-----------------------#

#-----------------------Begin ShaderEdit ----------------------#
# ShaderTreeEdit
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ShaderEdit/ShaderTreeEdit.h
	${MODULE_SOURCE_PATH}/ShaderEdit/ShaderTreeEdit.cpp
	"ShaderEdit\\\\Logic\\\\ShaderTreeEdit" 
)

# Logic
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ShaderEdit/SHLogic.h
	${MODULE_SOURCE_PATH}/ShaderEdit/SHLogic.cpp
	"ShaderEdit\\\\Logic\\\\" 
)

# MainWindow
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ShaderEdit/SHMainWindow.ui
	${MODULE_SOURCE_PATH}/ShaderEdit/SHMainWindow.h
	${MODULE_SOURCE_PATH}/ShaderEdit/SHMainWindow.cpp
	"ShaderEdit\\\\UI\\\\MainWindow"
	"ShaderEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# MaterialExpression
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ShaderEdit/MaterialExpression.ui
	${MODULE_SOURCE_PATH}/ShaderEdit/UIMaterialExpression.h
	${MODULE_SOURCE_PATH}/ShaderEdit/UIMaterialExpression.cpp
	"ShaderEdit\\\\UI\\\\MaterialExpression"
	"ShaderEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ParametersEdit
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ShaderEdit/ParametersEditor.ui
	${MODULE_SOURCE_PATH}/ShaderEdit/ParametersEditor.h
	${MODULE_SOURCE_PATH}/ShaderEdit/ParametersEditor.cpp
	"ShaderEdit\\\\UI\\\\ParametersEditor"
	"ShaderEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ShaderEdit
ADD_QTFORMS(
	""
	${MODULE_SOURCE_PATH}/ShaderEdit/TabWidget.h
	${MODULE_SOURCE_PATH}/ShaderEdit/TabWidget.cpp
	"ShaderEdit\\\\UI\\\\TabWidget"
	"ShaderEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ShaderTreeDisplay
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/ShaderEdit/ShaderTreeDisplay.ui
	${MODULE_SOURCE_PATH}/ShaderEdit/UIShaderTreeDisplay.h
	${MODULE_SOURCE_PATH}/ShaderEdit/UIShaderTreeDisplay.cpp
	"ShaderEdit\\\\UI\\\\ShaderTreeDisplay"
	"ShaderEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ShaderTreeControl
ADD_QTFORMS(
	""
	${MODULE_SOURCE_PATH}/ShaderEdit/ShaderTreeControl.h
	${MODULE_SOURCE_PATH}/ShaderEdit/ShaderTreeControl.cpp
	"ShaderEdit\\\\UI\\\\ShaderTreeDisplay\\\\Helper"
	"ShaderEdit\\\\UI\\\\Generated Files\\\\Moc"
)

# ShaderTreeConnectMgr
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ShaderEdit/ShaderTreeConnectMgr.h
	${MODULE_SOURCE_PATH}/ShaderEdit/ShaderTreeConnectMgr.cpp
	"ShaderEdit\\\\UI\\\\ShaderTreeDisplay\\\\Helper"
)

#-----------------------End   ShaderEdit ----------------------#

#-----------------------Start TextureTool----------------------#

# TextureSetBuild
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/TextureTool/TextureSetBuild.h
	${MODULE_SOURCE_PATH}/TextureTool/TextureSetBuild.cpp
	"TextureTool\\\\Logic\\\\"
)

# TextureTool
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/TextureTool/TextureTool.ui
	${MODULE_SOURCE_PATH}/TextureTool/TextureTool.h
	${MODULE_SOURCE_PATH}/TextureTool/TextureTool.cpp
	"TextureTool\\\\UI\\\\"
	"TextureTool\\\\UI\\\\Generated Files\\\\Moc"
)

#-----------------------End	  TextureTool----------------------#

#-----------------------Start UIEdit     ----------------------#

# UIEdit
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/UIEdit/UIEdit.h
	${MODULE_SOURCE_PATH}/UIEdit/UIEdit.cpp
	"UIEdit\\\\Logic\\\\"
)

# MainWindow
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/UIEdit/UIMainWindow.ui
	${MODULE_SOURCE_PATH}/UIEdit/UIMainWindow.h
	${MODULE_SOURCE_PATH}/UIEdit/UIMainWindow.cpp
	"UIEdit\\\\UI\\\\"
	"UIEdit\\\\UI\\\\Generated Files\\\\Moc"
)

#-----------------------End   UIEdit	 ----------------------#

#-----------------------Start Lightmass  ----------------------#
# Config
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/lightmass/ConfigMgr.h
	${MODULE_SOURCE_PATH}/lightmass/ConfigMgr.cpp
	"Lightmass\\\\Logic\\\\Config"
)

# PbrtTaskQueue
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/lightmass/PbrtTaskQueue.h
	${MODULE_SOURCE_PATH}/lightmass/PbrtTaskQueue.cpp
	"Lightmass\\\\Logic\\\\PbrtTaskQueue"
)

# Gallery
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/lightmass/Gallery.ui
	${MODULE_SOURCE_PATH}/lightmass/Gallery.h
	${MODULE_SOURCE_PATH}/lightmass/Gallery.cpp
	"Lightmass\\\\UI\\\\Gallery"
	"Lightmass\\\\UI\\\\Generated Files\\\\Moc"
)

# MainWindow
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/lightmass/LMMainWindow.ui
	${MODULE_SOURCE_PATH}/lightmass/LMMainWindow.h
	${MODULE_SOURCE_PATH}/lightmass/LMMainWindow.cpp
	"Lightmass\\\\UI\\\\MainWindow"
	"Lightmass\\\\UI\\\\Generated Files\\\\Moc"
)

#-----------------------End   Lightmass  ----------------------#

#-----------------------Start AStudio--------------------------#
SET(MODULE_AS_LOGIC_FILES
	${MODULE_SOURCE_PATH}/Application.h
	${MODULE_SOURCE_PATH}/Application.cpp
)
SOURCE_GROUP("AStudio\\Logic" FILES ${MODULE_AS_LOGIC_FILES})

# 主窗口
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/MainWindow.ui
	${MODULE_SOURCE_PATH}/MainWindow.h
	${MODULE_SOURCE_PATH}/MainWindow.cpp
	"AStudio\\\\UI\\\\MainWindow"
	"AStudio\\\\UI\\\\Generated Files\\\\Moc"
)

#LogPanel
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/LogPanel.ui
	${MODULE_SOURCE_PATH}/LogPanel.h
	${MODULE_SOURCE_PATH}/LogPanel.cpp
	"AStudio\\\\UI\\\\LogPanel"
	"AStudio\\\\UI\\\\Generated Files\\\\Moc"
)

# MdiArea
ADD_QTFORMS(
	""
	${MODULE_SOURCE_PATH}/MdiArea.h
	${MODULE_SOURCE_PATH}/MdiArea.cpp
	"AStudio\\\\UI\\\\MdiArea"
	"AStudio\\\\UI\\\\Generated Files\\\\Moc"
)

# RenderWidget
ADD_QTFORMS(
	""
	${MODULE_SOURCE_PATH}/RenderWidget.h
	${MODULE_SOURCE_PATH}/RenderWidget.cpp
	"AStudio\\\\UI\\\\RenderWidget"
	"AStudio\\\\UI\\\\Generated Files\\\\Moc"
)
#------------------------ End Astudio ------------------------------------#

# Qt 包含
INCLUDE(${QT_USE_FILE})

# Qt 定义
ADD_DEFINITIONS(${QT_DEFINITIONS})

# main文件
SET(MODULE_MAIN_SOURCE_FILES 
	${MODULE_SOURCE_PATH}/Main.cpp
)
SOURCE_GROUP("" FILES ${MODULE_MAIN_SOURCE_FILES})

# 添加库
ADD_EXECUTABLE(${MODULE_NAME} 
	${MODULE_ICON_FILE}
	${LOGIC_ALL_SOURCE_FILES}
	${MODULE_UI_FORMS_GENERATORS}
	${MODULE_UI_HEADERS_MOC}
	${MODULE_UI_LIBRARY}
	${MODULE_UI_FILES}
	${MODULE_UI_CPP_LIBRARY}
	${MODULE_UI_CPP_FILES}
	${MODULE_RCC_SRCS}
	${MODULE_MAIN_SOURCE_FILES}
	${MODULE_ASTUDIO_FILES}
	${MODULE_ME_FILES}
	${MODULE_MO_LOGIC_FILES}
	${MODULE_AS_LOGIC_FILES}
	${ASTUDIO_QT_FILES}
	${ASTUDIO_GROUP_FILES}
)

# 链接库
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(${MODULE_NAME} AresCore)
TARGET_LINK_LIBRARIES(${MODULE_NAME} AresPhysics)
TARGET_LINK_LIBRARIES(${MODULE_NAME} Ares3DEngine)
TARGET_LINK_LIBRARIES(${MODULE_NAME} tolua++)
TARGET_LINK_LIBRARIES(${MODULE_NAME} pbrt)
TARGET_LINK_LIBRARIES(${MODULE_NAME} EditorKit)

IF(MSVC)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} dxguid dinput8 d3d9 d3dx9)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} debug fmodexL_vc  optimized fmodex_vc)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} debug freetype_d  optimized freetype)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} Plugin_D3D11)
ENDIF(MSVC)

# 设置属性
SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES FOLDER "Editors")

# 设置安装目录
INSTALL_TARGETS(${EXECUTABLE_OUTPUT_PATH} ${MODULE_NAME})
INSTALL(TARGETS ${MODULE_NAME}
	RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)

#ADD_CUSTOM_COMMAND(TARGET ${MODULE_NAME} PRE_BUILD WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

