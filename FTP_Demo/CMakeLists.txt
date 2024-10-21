cmake_minimum_required(VERSION 3.8)

project(FTP_Demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
# Qt end

# 设置输出目录
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})

# 项目构建
set(PROJECT_SRC
    "main.cpp"
    "FtpManager.cpp")
add_executable(${PROJECT_NAME} ${PROJECT_SRC})

# 添加预处理器定义
target_compile_definitions(${PROJECT_NAME} PUBLIC CURL_STATICLIB)

# 根据编译选项设置不同的lib名称
if(CMAKE_BUILD_TYPE STREQUAL "Debug")  
    set(LIB_CURL_NAME "libcurl_a_debug.lib")  
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")  
    set(LIB_CURL_NAME "libcurl_a.lib")  
endif()
message("LIB_CURL=${LIB_CURL_NAME}")

# 链接外部库
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Ws2_32.lib
    Wldap32.lib
    winmm.lib
    Crypt32.lib
    Normaliz.lib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 对特殊的库单独链接
set(MY_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
find_library(CURL_LIB ${LIB_CURL_NAME} PATHS ${MY_LIB_PATH} NO_DEFAULT_PATH)  
if(CURL_LIB)  
    # 链接库  
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIB})
    message("Find lib：[${LIB_CURL_NAME}] path=${CURL_LIB}")  
else()  
    message("Could not find the library:${LIB_CURL}")  
endif()

