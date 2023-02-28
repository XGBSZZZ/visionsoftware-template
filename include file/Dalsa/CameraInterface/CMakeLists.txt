project(CameraInterface)
add_compile_options("/std:c++17")
cmake_minimum_required( VERSION 2.8 )

SET(OPENCV_DIR "INPUT" CACHE PATH "OPENCV_DIR")

INCLUDE(${OPENCV_DIR}/OpenCVConfig.cmake)

include_directories( ${OpenCV_INCLUDE_DIRS} )

SET(CAMERA_TYPE Basler CACHE STRING "CAMERA TYPE")
set_property(CACHE CAMERA_TYPE PROPERTY STRINGS Basler MindVision Baumer)

#add qt lib
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
#include_directories(${Qt5_include_dirs})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(BUILD_SHARED_LIBS OFF CACHE BOOL "BUILD_SHARED_LIBS")
#DEBUG生成的结尾为Dd
set (CMAKE_DEBUG_POSTFIX d)

set (CAMERAINTERFACE_HEADER 
CameraInterface/camerainterface.hxx
CameraInterface/VirtualCameraInterface.hxx)

if(CAMERA_TYPE STREQUAL Basler)

    SET(PYLON_INCLUDE "INPUT" CACHE PATH "PYLON DIR")
    #SET(PYLON_LIBS_DIR "INPUT" CACHE PATH "PYLON_LIBS_DIR")

    #link_libraries(
    #    ${PYLON_LIBS_DIR}/GCBase_MD_VC141_v3_1_Basler_pylon.lib
    #    ${PYLON_LIBS_DIR}/GenApi_MD_VC141_v3_1_Basler_pylon.lib
    #    ${PYLON_LIBS_DIR}/PylonBase_v5_2.lib
    #    ${PYLON_LIBS_DIR}/PylonC.lib
    #    ${PYLON_LIBS_DIR}/PylonGUI_v5_2.lib
    #    ${PYLON_LIBS_DIR}/PylonUtility_v5_2.lib
    #    )

    

    include_directories( ${PYLON_INCLUDE} )

    set(HEADERS 

    BaslerCameraInterface/baslercamerainterface.hxx
    )

    set(SOURCES 
    BaslerCameraInterface/baslercamerainterface.cxx
    )

    set(UIS 
    BaslerCameraInterface/BaslerCameraSettingDialog.ui
    )

    add_library(${CAMERA_TYPE}${PROJECT_NAME}  ${HEADERS} ${SOURCES} ${UIS} ${CAMERAINTERFACE_HEADER})

    #target_link_libraries(${CAMERA_TYPE}${PROJECT_NAME} Qt5::Widgets)
    #target_link_libraries(${CAMERA_TYPE}${PROJECT_NAME} ${OpenCV_LIBS})

    install (TARGETS ${CAMERA_TYPE}${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install/lib
    ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install/lib
    )

    

endif()
    install(FILES ${HEADERS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install/include/${CAMERA_TYPE}${PROJECT_NAME}/)
    install(FILES ${CAMERAINTERFACE_HEADER} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install/include/CameraInterface/)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CameraInterfaceConfig.cmake DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install/)