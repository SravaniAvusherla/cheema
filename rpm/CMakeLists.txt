#ADD_EXECUTABLE(rpm rpm.c)
#TARGET_LINK_LIBRARIES(rpm hiredis uv)

cmake_minimum_required(VERSION 3.4.0)
project(rpm_stateless)


add_subdirectory(hiredis)
add_subdirectory(libuv)

add_executable(rpm rpm.c)
target_link_libraries(rpm PUBLIC hiredis uv)
