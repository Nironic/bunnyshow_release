cmake_minimum_required(VERSION 3.10)
project(BunnyShow)

set(CMAKE_CXX_STANDARD 11)

# Скажи CMake где искать заголовочные файлы
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# Все .cpp файлы в текущей папке
file(GLOB SOURCES "*.cpp")

add_executable(bunnyshow ${SOURCES})

# Подключаем библиотеки
target_link_libraries(bunnyshow 
    raylib
    opengl32
    gdi32
    winmm
)

# Копируем assets в папку со сборкой
add_custom_command(TARGET bunnyshow POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:bunnyshow>/assets
)

message("✅ CMakeLists готов. Файлы: ${SOURCES}")