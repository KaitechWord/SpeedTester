find_package(Boost 1.82.0 COMPONENTS REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include(src/Server/CMakeLists.txt)
include(src/Client/CMakeLists.txt)
include(src/ThreadPool/CMakeLists.txt)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp ${server_src} ${client_src} ${thread_pool_src})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})