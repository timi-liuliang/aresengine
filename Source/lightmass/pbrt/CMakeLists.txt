# 库依赖

IF(WIN32)
	ADD_DEFINITIONS(-DPBRT_PROBES_NONE)
	ADD_DEFINITIONS(-DPBRT_HAS_OPENEXR)
	ADD_DEFINITIONS(-DZLIB_WINAPI)
ENDIF(WIN32)

# 附加包含目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt/core)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt/3rdparty)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt/3rdparty/ilmbase-1.0.2)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt/3rdparty/openexr-1.7.0)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt/3rdparty/zlib-1.2.5)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt/3rdparty/tiff-3.9.4)

# 附加库目录

# 设置模块名称
SET(MODULE_NAME pbrt)

# 源文件路径
SET(MODULE_HEADER_PATH ${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt)
SET(MODULE_SOURCE_PATH ${PROJECT_SOURCE_DIR}/Source/lightmass/pbrt)

# MSVC禁用特定警告(Visual Studio 8 and above)
IF(MSVC)
	SET(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG} /wd4996 /MP")
    SET(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE} /wd4996 /MP")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /wd4996")
    SET(CMAKE_C_FLAGS_DEBUG				"${CMAKE_C_FLAGS_DEBUG} /wd4996")
    SET(CMAKE_C_FLAGS_RELEASE			"${CMAKE_C_FLAGS_RELEASE} /wd4996")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO	"${CMAKE_C_FLAGS_RELWITHDEBINFO} /wd4996")

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

# 3rdparty ilmbase-1.0.2
SET(GROUP_ILMBASE_SOURCE_FILES
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/half.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/half.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/halfFunction.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/halfLimits.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/Iex.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IexBaseExc.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IexBaseExc.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IexErrnoExc.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IexMacros.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IexMathExc.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IexThrowErrnoExc.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IexThrowErrnoExc.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmBaseConfig.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThread.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThread.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadMutex.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadMutex.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadMutexPosix.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadMutexWin32.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadpool.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadPool.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadPosix.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadSemaphore.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadSemaphore.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadSemaphorePosix.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadSemaphorePosixCompat.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadSemaphoreWin32.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/IlmThreadWin32.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathBox.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathBox.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathBoxAlgo.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathColor.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathColorAlgo.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathColorAlgo.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathEuler.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathExc.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathFrame.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathFrustum.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathFun.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathFun.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathGL.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathGLU.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathHalfLimits.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathInt64.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathInterval.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathLimits.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathLine.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathLineAlgo.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathMath.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathMatrix.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathMatrixAlgo.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathMatrixAlgo.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathPlane.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathPlatform.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathQuat.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathRandom.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathRandom.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathRoots.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathShear.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathShear.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathSphere.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathVec.cpp
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathVec.h
	${MODULE_SOURCE_PATH}/3rdparty/ilmbase-1.0.2/ImathVecAlgo.h
)
SOURCE_GROUP("3rdparty\\ilmbase-1.0.2" FILES ${GROUP_ILMBASE_SOURCE_FILES})

# openexr-1.7.0
SET(GROUP_OPENEXR_SOURCE_FILES
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/b44ExpLogTable.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfAcesFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfAcesFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfArray.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfAutoArray.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfB44Compressor.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfB44Compressor.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfBoxAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfBoxAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChannelList.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChannelList.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChannelListAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChannelListAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCheckedArithmetic.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChromaticities.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChromaticities.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChromaticitiesAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfChromaticitiesAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCompression.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCompressionAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCompressionAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCompressor.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCompressor.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfConvert.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfConvert.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCRgbaFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfCRgbaFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfDoubleAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfDoubleAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfEnvmap.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfEnvmap.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfEnvmapAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfEnvmapAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfFloatAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfFloatAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfFrameBuffer.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfFrameBuffer.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfFramesPerSecond.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfFramesPerSecond.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfHeader.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfHeader.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfHuf.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfHuf.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfInputFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfInputFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfInt64.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfIntAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfIntAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfIO.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfIO.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfKeyCode.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfKeyCode.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfKeyCodeAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfKeyCodeAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfLineOrder.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfLineOrderAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfLineOrderAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfLut.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfLut.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfMatrixAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfMatrixAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfMisc.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfMisc.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfMultiView.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfMultiView.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfName.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfOpaqueAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfOpaqueAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfOutputFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPixelType.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPizCompressor.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPizCompressor.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPreviewImage.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPreviewImage.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPreviewImageAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPreviewImageAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPxr24Compressor.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfPxr24Compressor.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRational.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRational.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRationalAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRationalAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRgba.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRgbaFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRgbaFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRgbaYca.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRgbaYca.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRleCompressor.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfRleCompressor.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfScanLineInputFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfScanLineInputFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStandardAttributes.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStandardAttributes.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStdIO.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStdIO.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStringAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStringAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStringVectorAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfStringVectorAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTestFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTestFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfThreading.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfThreading.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTileDescription.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTileDescriptionAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTileDescriptionAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledInputFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledInputFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledMisc.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledMisc.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledOutputFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledOutputFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledRgbaFile.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTiledRgbaFile.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTileOffsets.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTileOffsets.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTimeCode.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTimeCode.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTimeCodeAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfTimeCodeAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfVecAttribute.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfVecAttribute.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfVersion.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfVersion.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfWav.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfWav.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfXdr.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfZipCompressor.cpp
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/ImfZipCompressor.h
	${MODULE_SOURCE_PATH}/3rdparty/openexr-1.7.0/OpenEXRConfig.h
)
SOURCE_GROUP("3rdparty\\openexr-1.7.0" FILES ${GROUP_OPENEXR_SOURCE_FILES})

# tiff-3.9.4
SET(GROUP_TIFF_SOURCE_FILES
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/mkg3states.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/t4.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_aux.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_close.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_codec.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_color.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_compress.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_dir.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_dir.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_dirinfo.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_dirread.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_dirwrite.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_dumpmode.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_error.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_extension.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_fax3.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_fax3.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_fax3sm.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_flush.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_getimage.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_jbig.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_jpeg.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_luv.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_lzw.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_next.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_ojpeg.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_open.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_packbits.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_pixarlog.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_predict.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_predict.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_print.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_read.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_stream.cxx
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_strip.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_swab.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_thunder.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_tile.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_version.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_warning.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_win32.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_write.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tif_zip.c
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiff.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiffconf.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiffconf.vc.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiffconf.wince.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiffio.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiffio.hxx
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiffiop.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/tiffvers.h
	${MODULE_SOURCE_PATH}/3rdparty/tiff-3.9.4/uvcode.h
)
SOURCE_GROUP("3rdparty\\tiff-3.9.4" FILES ${GROUP_TIFF_SOURCE_FILES})

# zlib
SET(GROUP_ZLIB_SOURCE_FILES
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/adler32.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/compress.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/crc32.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/crc32.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/deflate.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/deflate.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/gzclose.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/gzguts.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/gzlib.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/gzread.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/gzwrite.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/infback.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/inffast.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/inffast.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/inffixed.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/inflate.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/inflate.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/inftrees.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/inftrees.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/trees.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/trees.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/uncompr.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/zconf.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/zlib.h
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/zutil.c
	${MODULE_SOURCE_PATH}/3rdparty/zlib-1.2.5/zutil.h
)
SOURCE_GROUP("3rdparty\\zlib-1.2.5" FILES ${GROUP_ZLIB_SOURCE_FILES})

# unistd
SET(GROUP_3RDPARTY_SOURCE_FILES
	${MODULE_SOURCE_PATH}/3rdparty/unistd.h
)
SOURCE_GROUP("3rdparty" FILES ${GROUP_3RDPARTY_SOURCE_FILES})

# Accelerators
SET(GROUP_ACCELERATORS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/accelerators/bvh.h
	${MODULE_SOURCE_PATH}/accelerators/bvh.cpp
	${MODULE_SOURCE_PATH}/accelerators/grid.h
	${MODULE_SOURCE_PATH}/accelerators/grid.cpp
	${MODULE_SOURCE_PATH}/accelerators/kdtreeaccel.h
	${MODULE_SOURCE_PATH}/accelerators/kdtreeaccel.cpp
)
SOURCE_GROUP("accelerators" FILES ${GROUP_ACCELERATORS_SOURCE_FILES})

# cameras
SET(GROUP_CAMERAS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/cameras/environment.h
	${MODULE_SOURCE_PATH}/cameras/environment.cpp
	${MODULE_SOURCE_PATH}/cameras/orthographic.h
	${MODULE_SOURCE_PATH}/cameras/orthographic.cpp
	${MODULE_SOURCE_PATH}/cameras/perspective.h
	${MODULE_SOURCE_PATH}/cameras/perspective.cpp
)
SOURCE_GROUP("cameras" FILES ${GROUP_CAMERAS_SOURCE_FILES})

# core
SET(GROUP_CORE_SOURCE_FILES
	${MODULE_SOURCE_PATH}/core/api.h
	${MODULE_SOURCE_PATH}/core/api.cpp
	${MODULE_SOURCE_PATH}/core/camera.h
	${MODULE_SOURCE_PATH}/core/camera.cpp
	${MODULE_SOURCE_PATH}/core/diffgeom.h
	${MODULE_SOURCE_PATH}/core/diffgeom.cpp
	${MODULE_SOURCE_PATH}/core/dtrace.d
	${MODULE_SOURCE_PATH}/core/error.h
	${MODULE_SOURCE_PATH}/core/error.cpp
	${MODULE_SOURCE_PATH}/core/fileutil.h
	${MODULE_SOURCE_PATH}/core/fileutil.cpp
	${MODULE_SOURCE_PATH}/core/film.h
	${MODULE_SOURCE_PATH}/core/film.cpp
	${MODULE_SOURCE_PATH}/core/filter.h
	${MODULE_SOURCE_PATH}/core/filter.cpp
	${MODULE_SOURCE_PATH}/core/floatfile.h
	${MODULE_SOURCE_PATH}/core/floatfile.cpp
	${MODULE_SOURCE_PATH}/core/geometry.h
	${MODULE_SOURCE_PATH}/core/geometry.cpp
	${MODULE_SOURCE_PATH}/core/imageio.h
	${MODULE_SOURCE_PATH}/core/imageio.cpp
	${MODULE_SOURCE_PATH}/core/integrator.h
	${MODULE_SOURCE_PATH}/core/integrator.cpp
	${MODULE_SOURCE_PATH}/core/intersection.h
	${MODULE_SOURCE_PATH}/core/intersection.cpp
	${MODULE_SOURCE_PATH}/core/kdtree.h
	${MODULE_SOURCE_PATH}/core/light.h
	${MODULE_SOURCE_PATH}/core/light.cpp
	${MODULE_SOURCE_PATH}/core/material.h
	${MODULE_SOURCE_PATH}/core/material.cpp
	${MODULE_SOURCE_PATH}/core/memory.h
	${MODULE_SOURCE_PATH}/core/memory.cpp
	${MODULE_SOURCE_PATH}/core/mipmap.h
	${MODULE_SOURCE_PATH}/core/montecarlo.h
	${MODULE_SOURCE_PATH}/core/montecarlo.cpp
	${MODULE_SOURCE_PATH}/core/octree.h
	${MODULE_SOURCE_PATH}/core/parallel.h
	${MODULE_SOURCE_PATH}/core/parallel.cpp
	${MODULE_SOURCE_PATH}/core/paramset.h
	${MODULE_SOURCE_PATH}/core/paramset.cpp
	${MODULE_SOURCE_PATH}/core/parser.h
	${MODULE_SOURCE_PATH}/core/parser.cpp
	${MODULE_SOURCE_PATH}/core/pbrt.h
	${MODULE_SOURCE_PATH}/core/pbrtlex.cpp
	${MODULE_SOURCE_PATH}/core/pbrtlex.ll
	${MODULE_SOURCE_PATH}/core/pbrtparse.hpp
	${MODULE_SOURCE_PATH}/core/pbrtparse.cpp
	${MODULE_SOURCE_PATH}/core/pbrtparse.yy
	${MODULE_SOURCE_PATH}/core/primitive.h
	${MODULE_SOURCE_PATH}/core/primitive.cpp
	${MODULE_SOURCE_PATH}/core/probes.h
	${MODULE_SOURCE_PATH}/core/probes.cpp
	${MODULE_SOURCE_PATH}/core/progressreporter.h
	${MODULE_SOURCE_PATH}/core/progressreporter.cpp
	${MODULE_SOURCE_PATH}/core/quaternion.h
	${MODULE_SOURCE_PATH}/core/quaternion.cpp
	${MODULE_SOURCE_PATH}/core/reflection.h
	${MODULE_SOURCE_PATH}/core/reflection.cpp
	${MODULE_SOURCE_PATH}/core/renderer.h
	${MODULE_SOURCE_PATH}/core/renderer.cpp
	${MODULE_SOURCE_PATH}/core/rng.h
	${MODULE_SOURCE_PATH}/core/rng.cpp
	${MODULE_SOURCE_PATH}/core/sampler.h
	${MODULE_SOURCE_PATH}/core/sampler.cpp
	${MODULE_SOURCE_PATH}/core/scene.h
	${MODULE_SOURCE_PATH}/core/scene.cpp
	${MODULE_SOURCE_PATH}/core/sh.h
	${MODULE_SOURCE_PATH}/core/sh.cpp
	${MODULE_SOURCE_PATH}/core/shape.h
	${MODULE_SOURCE_PATH}/core/shape.cpp
	${MODULE_SOURCE_PATH}/core/shrots.cpp
	${MODULE_SOURCE_PATH}/core/spectrum.h
	${MODULE_SOURCE_PATH}/core/spectrum.cpp
	${MODULE_SOURCE_PATH}/core/stdafx.h
	${MODULE_SOURCE_PATH}/core/texture.h
	${MODULE_SOURCE_PATH}/core/texture.cpp
	${MODULE_SOURCE_PATH}/core/timer.h
	${MODULE_SOURCE_PATH}/core/timer.cpp
	${MODULE_SOURCE_PATH}/core/transform.h
	${MODULE_SOURCE_PATH}/core/transform.cpp
	${MODULE_SOURCE_PATH}/core/volume.h
	${MODULE_SOURCE_PATH}/core/volume.cpp
)
SOURCE_GROUP("core" FILES ${GROUP_CORE_SOURCE_FILES})

# film
SET(GROUP_FILM_SOURCE_FILES
	${MODULE_SOURCE_PATH}/film/image.h
	${MODULE_SOURCE_PATH}/film/image.cpp
)
SOURCE_GROUP("film" FILES ${GROUP_FILM_SOURCE_FILES})

# filters
SET(GROUP_FILTERS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/filters/box.h
	${MODULE_SOURCE_PATH}/filters/box.cpp
	${MODULE_SOURCE_PATH}/filters/gaussian.h
	${MODULE_SOURCE_PATH}/filters/gaussian.cpp
	${MODULE_SOURCE_PATH}/filters/mitchell.h
	${MODULE_SOURCE_PATH}/filters/mitchell.cpp
	${MODULE_SOURCE_PATH}/filters/sinc.h
	${MODULE_SOURCE_PATH}/filters/sinc.cpp
	${MODULE_SOURCE_PATH}/filters/triangle.h
	${MODULE_SOURCE_PATH}/filters/triangle.cpp
)
SOURCE_GROUP("filters" FILES ${GROUP_FILTERS_SOURCE_FILES})

# integrators
SET(GROUP_INTEGRATORS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/integrators/ambientocclusion.h
	${MODULE_SOURCE_PATH}/integrators/ambientocclusion.cpp
	${MODULE_SOURCE_PATH}/integrators/diffuseprt.h
	${MODULE_SOURCE_PATH}/integrators/diffuseprt.cpp
	${MODULE_SOURCE_PATH}/integrators/dipolesubsurface.h
	${MODULE_SOURCE_PATH}/integrators/dipolesubsurface.cpp
	${MODULE_SOURCE_PATH}/integrators/directlighting.h
	${MODULE_SOURCE_PATH}/integrators/directlighting.cpp
	${MODULE_SOURCE_PATH}/integrators/emission.h
	${MODULE_SOURCE_PATH}/integrators/emission.cpp
	${MODULE_SOURCE_PATH}/integrators/glossyprt.h
	${MODULE_SOURCE_PATH}/integrators/glossyprt.cpp
	${MODULE_SOURCE_PATH}/integrators/igi.h
	${MODULE_SOURCE_PATH}/integrators/igi.cpp
	${MODULE_SOURCE_PATH}/integrators/irradiancecache.h
	${MODULE_SOURCE_PATH}/integrators/irradiancecache.cpp
	${MODULE_SOURCE_PATH}/integrators/path.h
	${MODULE_SOURCE_PATH}/integrators/path.cpp
	${MODULE_SOURCE_PATH}/integrators/photonmap.h
	${MODULE_SOURCE_PATH}/integrators/photonmap.cpp
	${MODULE_SOURCE_PATH}/integrators/single.h
	${MODULE_SOURCE_PATH}/integrators/single.cpp
	${MODULE_SOURCE_PATH}/integrators/useprobes.h
	${MODULE_SOURCE_PATH}/integrators/useprobes.cpp
	${MODULE_SOURCE_PATH}/integrators/whitted.h
	${MODULE_SOURCE_PATH}/integrators/whitted.cpp
)
SOURCE_GROUP("integrators" FILES ${GROUP_INTEGRATORS_SOURCE_FILES})

# lights
SET(GROUP_LIGHTS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/lights/diffuse.h
	${MODULE_SOURCE_PATH}/lights/diffuse.cpp
	${MODULE_SOURCE_PATH}/lights/distant.h
	${MODULE_SOURCE_PATH}/lights/distant.cpp
	${MODULE_SOURCE_PATH}/lights/goniometric.h
	${MODULE_SOURCE_PATH}/lights/goniometric.cpp
	${MODULE_SOURCE_PATH}/lights/infinite.h
	${MODULE_SOURCE_PATH}/lights/infinite.cpp
	${MODULE_SOURCE_PATH}/lights/point.h
	${MODULE_SOURCE_PATH}/lights/point.cpp
	${MODULE_SOURCE_PATH}/lights/projection.h
	${MODULE_SOURCE_PATH}/lights/projection.cpp
	${MODULE_SOURCE_PATH}/lights/spot.h
	${MODULE_SOURCE_PATH}/lights/spot.cpp
)
SOURCE_GROUP("lights" FILES ${GROUP_LIGHTS_SOURCE_FILES})

# materials
SET(GROUP_MATERIAL_SOURCE_FILES
	${MODULE_SOURCE_PATH}/materials/glass.h
	${MODULE_SOURCE_PATH}/materials/glass.cpp
	${MODULE_SOURCE_PATH}/materials/kdsubsurface.h
	${MODULE_SOURCE_PATH}/materials/kdsubsurface.cpp
	${MODULE_SOURCE_PATH}/materials/matte.h
	${MODULE_SOURCE_PATH}/materials/matte.cpp
	${MODULE_SOURCE_PATH}/materials/measured.h
	${MODULE_SOURCE_PATH}/materials/measured.cpp
	${MODULE_SOURCE_PATH}/materials/metal.h
	${MODULE_SOURCE_PATH}/materials/metal.cpp
	${MODULE_SOURCE_PATH}/materials/mirror.h
	${MODULE_SOURCE_PATH}/materials/mirror.cpp
	${MODULE_SOURCE_PATH}/materials/mixmat.h
	${MODULE_SOURCE_PATH}/materials/mixmat.cpp
	${MODULE_SOURCE_PATH}/materials/plastic.h
	${MODULE_SOURCE_PATH}/materials/plastic.cpp
	${MODULE_SOURCE_PATH}/materials/shinymetal.h
	${MODULE_SOURCE_PATH}/materials/shinymetal.cpp
	${MODULE_SOURCE_PATH}/materials/substrate.h
	${MODULE_SOURCE_PATH}/materials/substrate.cpp
	${MODULE_SOURCE_PATH}/materials/subsurface.h
	${MODULE_SOURCE_PATH}/materials/subsurface.cpp
	${MODULE_SOURCE_PATH}/materials/translucent.h
	${MODULE_SOURCE_PATH}/materials/translucent.cpp
	${MODULE_SOURCE_PATH}/materials/uber.h
	${MODULE_SOURCE_PATH}/materials/uber.cpp
)
SOURCE_GROUP("materials" FILES ${GROUP_MATERIAL_SOURCE_FILES})

# renderers
SET(GROUP_RENDERERS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/renderers/aggregatetest.h
	${MODULE_SOURCE_PATH}/renderers/aggregatetest.cpp
	${MODULE_SOURCE_PATH}/renderers/createprobes.h
	${MODULE_SOURCE_PATH}/renderers/createprobes.cpp
	${MODULE_SOURCE_PATH}/renderers/metropolis.h
	${MODULE_SOURCE_PATH}/renderers/metropolis.cpp
	${MODULE_SOURCE_PATH}/renderers/samplerrenderer.h
	${MODULE_SOURCE_PATH}/renderers/samplerrenderer.cpp
	${MODULE_SOURCE_PATH}/renderers/surfacepoints.h
	${MODULE_SOURCE_PATH}/renderers/surfacepoints.cpp
)
SOURCE_GROUP("renderers" FILES ${GROUP_RENDERERS_SOURCE_FILES})

# samplers
SET(GROUP_SAMPLERS_SOURCE_FILES
	${MODULE_SOURCE_PATH}/samplers/adaptive.h
	${MODULE_SOURCE_PATH}/samplers/adaptive.cpp
	${MODULE_SOURCE_PATH}/samplers/bestcandidate.h
	${MODULE_SOURCE_PATH}/samplers/bestcandidate.cpp
	${MODULE_SOURCE_PATH}/samplers/halton.h
	${MODULE_SOURCE_PATH}/samplers/halton.cpp
	${MODULE_SOURCE_PATH}/samplers/lowdiscrepancy.h
	${MODULE_SOURCE_PATH}/samplers/lowdiscrepancy.cpp
	${MODULE_SOURCE_PATH}/samplers/random.h
	${MODULE_SOURCE_PATH}/samplers/random.cpp
	${MODULE_SOURCE_PATH}/samplers/stratified.h
	${MODULE_SOURCE_PATH}/samplers/stratified.cpp
)
SOURCE_GROUP("samplers" FILES ${GROUP_SAMPLERS_SOURCE_FILES})

# shapes
SET(GROUP_SHAPES_SOURCE_FILES
	${MODULE_SOURCE_PATH}/shapes/cone.h
	${MODULE_SOURCE_PATH}/shapes/cone.cpp
	${MODULE_SOURCE_PATH}/shapes/cylinder.h
	${MODULE_SOURCE_PATH}/shapes/cylinder.cpp
	${MODULE_SOURCE_PATH}/shapes/disk.h
	${MODULE_SOURCE_PATH}/shapes/disk.cpp
	${MODULE_SOURCE_PATH}/shapes/heightfield.h
	${MODULE_SOURCE_PATH}/shapes/heightfield.cpp
	${MODULE_SOURCE_PATH}/shapes/hyperboloid.h
	${MODULE_SOURCE_PATH}/shapes/hyperboloid.cpp
	${MODULE_SOURCE_PATH}/shapes/loopsubdiv.h
	${MODULE_SOURCE_PATH}/shapes/loopsubdiv.cpp
	${MODULE_SOURCE_PATH}/shapes/nurbs.h
	${MODULE_SOURCE_PATH}/shapes/nurbs.cpp
	${MODULE_SOURCE_PATH}/shapes/paraboloid.h
	${MODULE_SOURCE_PATH}/shapes/paraboloid.cpp
	${MODULE_SOURCE_PATH}/shapes/sphere.h
	${MODULE_SOURCE_PATH}/shapes/sphere.cpp
	${MODULE_SOURCE_PATH}/shapes/trianglemesh.h
	${MODULE_SOURCE_PATH}/shapes/trianglemesh.cpp
)
SOURCE_GROUP("shapes" FILES ${GROUP_SHAPES_SOURCE_FILES})

# textures
SET(GROUP_TEXTURES_SOURCE_FILES
	${MODULE_SOURCE_PATH}/textures/bilerp.h
	${MODULE_SOURCE_PATH}/textures/bilerp.cpp
	${MODULE_SOURCE_PATH}/textures/checkerboard.h
	${MODULE_SOURCE_PATH}/textures/checkerboard.cpp
	${MODULE_SOURCE_PATH}/textures/constant.h
	${MODULE_SOURCE_PATH}/textures/constant.cpp
	${MODULE_SOURCE_PATH}/textures/dots.h
	${MODULE_SOURCE_PATH}/textures/dots.cpp
	${MODULE_SOURCE_PATH}/textures/fbm.h
	${MODULE_SOURCE_PATH}/textures/fbm.cpp
	${MODULE_SOURCE_PATH}/textures/imagemap.h
	${MODULE_SOURCE_PATH}/textures/imagemap.cpp
	${MODULE_SOURCE_PATH}/textures/marble.h
	${MODULE_SOURCE_PATH}/textures/marble.cpp
	${MODULE_SOURCE_PATH}/textures/mix.h
	${MODULE_SOURCE_PATH}/textures/mix.cpp
	${MODULE_SOURCE_PATH}/textures/scale.h
	${MODULE_SOURCE_PATH}/textures/scale.cpp
	${MODULE_SOURCE_PATH}/textures/uv.h
	${MODULE_SOURCE_PATH}/textures/uv.cpp
	${MODULE_SOURCE_PATH}/textures/windy.h
	${MODULE_SOURCE_PATH}/textures/windy.cpp
	${MODULE_SOURCE_PATH}/textures/wrinkled.h
	${MODULE_SOURCE_PATH}/textures/wrinkled.cpp
)
SOURCE_GROUP("textures" FILES ${GROUP_TEXTURES_SOURCE_FILES})

# volumes
SET(GROUP_VOLUMES_SOURCE_FILES
	${MODULE_SOURCE_PATH}/volumes/exponential.h
	${MODULE_SOURCE_PATH}/volumes/exponential.cpp
	${MODULE_SOURCE_PATH}/volumes/homogeneous.h
	${MODULE_SOURCE_PATH}/volumes/homogeneous.cpp
	${MODULE_SOURCE_PATH}/volumes/volumegrid.h
	${MODULE_SOURCE_PATH}/volumes/volumegrid.cpp
)
SOURCE_GROUP("volumes" FILES ${GROUP_VOLUMES_SOURCE_FILES})

# 添加库
ADD_LIBRARY(${MODULE_NAME}
	${GROUP_ILMBASE_SOURCE_FILES}
	${GROUP_OPENEXR_SOURCE_FILES}
	${GROUP_TIFF_SOURCE_FILES}
	${GROUP_ZLIB_SOURCE_FILES}
	${GROUP_3RDPARTY_SOURCE_FILES}
	${GROUP_ACCELERATORS_SOURCE_FILES}
	${GROUP_CAMERAS_SOURCE_FILES}
	${GROUP_CORE_SOURCE_FILES}
	${GROUP_FILM_SOURCE_FILES}
	${GROUP_FILTERS_SOURCE_FILES}
	${GROUP_INTEGRATORS_SOURCE_FILES}
	${GROUP_LIGHTS_SOURCE_FILES}
	${GROUP_MATERIAL_SOURCE_FILES}
	${GROUP_RENDERERS_SOURCE_FILES}
	${GROUP_SAMPLERS_SOURCE_FILES}
	${GROUP_SHAPES_SOURCE_FILES}
	${GROUP_TEXTURES_SOURCE_FILES}
	${GROUP_VOLUMES_SOURCE_FILES}
)

# 设置属性
SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES FOLDER "Lightmass")