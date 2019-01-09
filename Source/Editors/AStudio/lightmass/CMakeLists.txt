
# 库依赖
FIND_PACKAGE(Qt4 REQUIRED)

# 设置模块名称
SET(MODULE_NAME lightmass)

# 附加包含目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass)

# 源文件路径
SET(MODULE_SOURCE_PATH		${PROJECT_SOURCE_DIR}/Source/lightmass/lightmass)

# 函数
MACRO(ADD_QTFORMS uiFile hFile cppFile group mocGroup)
	SET(QTUI_HEADER_UI)
	SET(UI_HEADERS_MOC)
	QT4_WRAP_UI(QTUI_HEADER_UI	${uiFile})
	QT4_WRAP_CPP(UI_HEADERS_MOC ${hFile})
	
	# UI 文件
	SOURCE_GROUP(${group}	 FILES ${hFile} ${cppFile} ${uiFile} ${QTUI_HEADER_UI})
	SOURCE_GROUP(${mocGroup} FILES ${UI_HEADERS_MOC})
	
	SET(MODULE_QT_FILES ${MODULE_QT_FILES} ${hFile} ${cppFile} ${uiFile} ${QTUI_HEADER_UI} ${UI_HEADERS_MOC})
ENDMACRO(ADD_QTFORMS)

#组文件
MACRO(ADD_GROUP2 file1 file2 groupName)
	# UI 文件
	SOURCE_GROUP(${groupName} FILES ${file1} ${file2})
	
	SET(MODULE_QT_FILES  ${MODULE_QT_FILES} ${file1} ${file2})
ENDMACRO(ADD_GROUP2)

# Config
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/ConfigMgr.h
	${MODULE_SOURCE_PATH}/ConfigMgr.cpp
	"Logic\\\\Config"
)

# PbrtTaskQueue
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/PbrtTaskQueue.h
	${MODULE_SOURCE_PATH}/PbrtTaskQueue.cpp
	"Logic\\\\PbrtTaskQueue"
)

# PbrtLoadTga
ADD_GROUP2(
	${MODULE_SOURCE_PATH}/LoadTga.h
	${MODULE_SOURCE_PATH}/LoadTga.cpp
	"UI\\\\QLibrary\\\\QtLoadTga"
)

# Gallery
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/Gallery.ui
	${MODULE_SOURCE_PATH}/Gallery.h
	${MODULE_SOURCE_PATH}/Gallery.cpp
	"UI\\\\Gallery"
	"UI\\\\Generated Files\\\\Moc"
)

# MainWindow
ADD_QTFORMS(
	${MODULE_SOURCE_PATH}/MainWindow.ui
	${MODULE_SOURCE_PATH}/MainWindow.h
	${MODULE_SOURCE_PATH}/MainWindow.cpp
	"UI\\\\MainWindow"
	"UI\\\\Generated Files\\\\Moc"
)

# PbrthightLight
ADD_QTFORMS(
	""
	${MODULE_SOURCE_PATH}/QPbrtSyntaxHighLighter.h
	${MODULE_SOURCE_PATH}/QPbrtSyntaxHighLighter.cpp
	"UI\\\\QLibrary\\\\PbrtHighLight"
	"UI\\\\Generated Files\\\\Moc"
)

# QT资源 图标
SET(MODULE_ICON_FILE ${MODULE_SOURCE_PATH}/App.rc)
SET(MODULE_QT_RESOURCE_PATH ${MODULE_SOURCE_PATH}/App.qrc)
QT4_ADD_RESOURCES(MODULE_RCC_SRCS ${MODULE_QT_RESOURCE_PATH})
SET(MODULE_RESOURCE_FILES ${MODULE_ICON_FILE} ${MODULE_RCC_SRCS} ${MODULE_QT_RESOURCE_PATH})

SOURCE_GROUP("UI\\Generated Files\\Resources\\"	FILES ${MODULE_ICON_FILE})
SOURCE_GROUP("UI\\Generated Files\\Resources\\" FILES ${MODULE_RCC_SRCS})
SOURCE_GROUP("UI\\Generated Files\\Resources\\" FILES ${MODULE_QT_RESOURCE_PATH})

# main
SET(MODULE_MAIN_SOURCE_FILES
	${MODULE_SOURCE_PATH}/Main.cpp
)
SOURCE_GROUP("" FILES ${MODULE_MAIN_SOURCE_FILES})

# 添加库
ADD_EXECUTABLE(${MODULE_NAME} 
	${MODULE_GROUP_FILES}
	${MODULE_QT_FILES}
	${MODULE_SOURCE_PATH}/Main.cpp
	${MODULE_RESOURCE_FILES}
)

# 链接库
TARGET_LINK_LIBRARIES(${MODULE_NAME} pbrt)

# 设置属性
SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES FOLDER "Lightmass")