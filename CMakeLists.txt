cmake_minimum_required (VERSION 3.1)
project (factories)

add_subdirectory(src)

#add custom libraries to project
include_directories(${PROJECT_SOURCE_DIR}/src)
link_directories(${PROJECT_SOURCE_DIR}/src)


#add sqlite3 to project
include_directories(/usr/local/include/sqlite3)
link_directories(/usr/local/lib)

#add Threads package
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)


#add main executable
add_executable (factories ${PROJECT_SOURCE_DIR}/app/factories.cpp)
target_link_libraries(factories factory)


#add exe for testing sqlite
add_executable(sqlitetest ${PROJECT_SOURCE_DIR}/app/sqlitetest.cpp)
target_link_libraries(sqlitetest Threads::Threads)  #link threads to exe
target_link_libraries(sqlitetest sqlitewrapper)
target_link_libraries(sqlitetest sqlite3)           #link sqlite3 library
target_link_libraries(sqlitetest ${CMAKE_DL_LIBS})  #link dllibs for sql3
