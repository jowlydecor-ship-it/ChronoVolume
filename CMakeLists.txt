cmake_minimum_required(VERSION 3.15)
project(ChronoVolume LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Path to a checkout of https://github.com/AcademySoftwareFoundation/openfx
set(OFX_SDK "" CACHE PATH "Path to the OpenFX SDK checkout")
if(NOT OFX_SDK)
  message(FATAL_ERROR "Pass -DOFX_SDK=/path/to/openfx")
endif()

set(SUPPORT ${OFX_SDK}/Support)
set(SUPPORT_SRC
  ${SUPPORT}/Library/ofxsCore.cpp
  ${SUPPORT}/Library/ofxsImageEffect.cpp
  ${SUPPORT}/Library/ofxsInteract.cpp
  ${SUPPORT}/Library/ofxsLog.cpp
  ${SUPPORT}/Library/ofxsMultiThread.cpp
  ${SUPPORT}/Library/ofxsParams.cpp
  ${SUPPORT}/Library/ofxsProperty.cpp
  ${SUPPORT}/Library/ofxsPropertyValidation.cpp)

find_package(Threads REQUIRED)

add_library(ChronoVolume MODULE ChronoVolume.cpp chrono_core.h ${SUPPORT_SRC})
target_include_directories(ChronoVolume PRIVATE ${OFX_SDK}/include ${SUPPORT}/include)
target_link_libraries(ChronoVolume PRIVATE Threads::Threads)
set_target_properties(ChronoVolume PROPERTIES PREFIX "" SUFFIX ".ofx")

# Assemble the .ofx.bundle next to the build output
set(BUNDLE ${CMAKE_BINARY_DIR}/ChronoVolume.ofx.bundle)
if(WIN32)
  set(ARCH_DIR Win64)
elseif(APPLE)
  set(ARCH_DIR MacOS)
else()
  set(ARCH_DIR Linux-x86-64)
endif()
add_custom_command(TARGET ChronoVolume POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${BUNDLE}/Contents/${ARCH_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ChronoVolume> ${BUNDLE}/Contents/${ARCH_DIR}/ChronoVolume.ofx)
if(APPLE)
  add_custom_command(TARGET ChronoVolume POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Info.plist ${BUNDLE}/Contents/Info.plist)
endif()
