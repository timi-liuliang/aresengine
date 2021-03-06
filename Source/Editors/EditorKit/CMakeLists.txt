# 库依赖
#FIND_PACKAGE(Boost 1.5.2 REQUIRED)

# 附加包含目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include)

# 添加预处理器
IF(WIN32)
	ADD_DEFINITIONS(-DPLATFORM_WIN32)
ELSEIF(APPLE)
	IF(${CMAKE_OSX_SYSROOT} MATCHES "MacOSX.platform")
		ADD_DEFINITIONS(-DPLATFORM_MAC)
	ELSE()
		ADD_DEFINITIONS(-DPLATFORM_IOS)
	ENDIF()
ENDIF()

# 设置模块名称
SET(MODULE_NAME EditorKit)

# 源文件路径
SET(MODULE_HEADER_PATH ${PROJECT_SOURCE_DIR}/Include/${MODULE_NAME})
SET(MODULE_SOURCE_PATH ${PROJECT_SOURCE_DIR}/Source/Editors/${MODULE_NAME})

# VS配置 http://www.oschina.net/code/explore/mariadb-5.2.4/CMakeLists.txt
IF(MSVC)
	# 禁用特定警告
	SET(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG} /wd4996 /wd4819")
    SET(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE} /wd4996 /wd4819")
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

# 包含源文件
SET(MODULE_HEADER_FILES
	${MODULE_SOURCE_PATH}/Ares3DAxis.h
	${MODULE_SOURCE_PATH}/AresEditorKit.h
	${MODULE_SOURCE_PATH}/AresWaterEk.h
	${MODULE_SOURCE_PATH}/AresWorldEK.h
	${MODULE_SOURCE_PATH}/BaseCamera.h
	${MODULE_SOURCE_PATH}/DataTypeConversion.h
	${MODULE_SOURCE_PATH}/FollowViewerCamera.h
	${MODULE_SOURCE_PATH}/FxEditCamera.h
	${MODULE_SOURCE_PATH}/ReflectionCamera.h
	${MODULE_SOURCE_PATH}/ThirdViewCamera.h
	${MODULE_SOURCE_PATH}/BulletDebugDrawer.h
)

SET(MODULE_SOURCE_FILES
	${MODULE_SOURCE_PATH}/Ares3DAxis.cpp
	${MODULE_SOURCE_PATH}/BaseCamera.cpp
	${MODULE_SOURCE_PATH}/DataTypeConversion.cpp
	${MODULE_SOURCE_PATH}/FollowViewCamera.cpp
	${MODULE_SOURCE_PATH}/FxEditCamera.cpp
	${MODULE_SOURCE_PATH}/ReflectionCamera.cpp
	${MODULE_SOURCE_PATH}/ThirdViewCamera.cpp
	${MODULE_SOURCE_PATH}/BulletDebugDrawer.cpp
)

SOURCE_GROUP("Source Files" FILES ${MODULE_SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${MODULE_HEADER_FILES})

# 添加库
ADD_LIBRARY(${MODULE_NAME} 
	${MODULE_HEADER_FILES}
	${MODULE_SOURCE_FILES}
)

# 设置属性
SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES FOLDER "Editors")