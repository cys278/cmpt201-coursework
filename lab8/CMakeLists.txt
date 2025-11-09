cmake_minimum_required(VERSION 3.16)
project(lab8 C)
set(CMAKE_C_STANDARD 11)

add_executable(main lab8.c)

#pthreads
find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)

#uthash is header-nly; include current dir
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ------ ThreadSanitizer (TASK 3) ------
#add_compile_options(-fsanitize=thread -g -O1)
#add_link_options(-fsanitize=thread -g -O1)


