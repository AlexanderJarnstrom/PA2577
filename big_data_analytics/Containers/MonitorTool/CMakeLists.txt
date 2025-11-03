cmake_minimum_required (VERSION 3.15)
project(monitor-tool LANGUAGES CXX)

find_package(bsoncxx 4.1 REQUIRED)
find_package(mongocxx 4.1 REQUIRED)
 
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/database.cpp
  src/frontend.cpp 
  src/statistics.cpp 
  src/statistics_manager.cpp
  src/log_manager.cpp
)

include_directories( ${PROJECT_NAME}
  /usr/local/include/bsoncxx/v_noabi/
  /usr/local/include/mongocxx/v_noabi/
  /usr/include/ftxui/
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE /usr/local/lib/libftxui-component.a
  PRIVATE /usr/local/lib/libftxui-dom.a
  PRIVATE /usr/local/lib/libftxui-screen.a
  PRIVATE mongo::bsoncxx_static
  PRIVATE mongo::mongocxx_static
)
