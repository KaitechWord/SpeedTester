find_package(Boost 1.82.0 COMPONENTS REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

message(Boost_INCLUDE_DIRS="${Boost_INCLUDE_DIRS}")
message(Boost_LIBRARY_DIRS="${Boost_LIBRARY_DIRS}")

include(src/Server/CMakeLists.txt)
include(src/Client/CMakeLists.txt)
include(src/ThreadPool/CMakeLists.txt)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Server src/server.cpp ${server_src} ${thread_pool_src})

add_executable(Client1 src/client.cpp ${client_src} ${thread_pool_src})

add_executable(Client2 src/client.cpp ${client_src} ${thread_pool_src})

target_compile_features(Server PRIVATE cxx_std_20)
target_compile_features(Client1 PRIVATE cxx_std_20)
target_compile_features(Client2 PRIVATE cxx_std_20)

target_link_libraries(Server LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(Client1 LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(Client2 LINK_PUBLIC ${Boost_LIBRARIES})