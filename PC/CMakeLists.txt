cmake_minimum_required(VERSION 2.8.11)

project(WhacAMaker)

# Find the QtQml library
find_package(Qt5Qml 5.3 REQUIRED)
find_package(Qt5Quick 5.3 REQUIRED)
find_package(Qt5Gui 5.3 REQUIRED)
find_package(Qt5SerialPort 5.3 REQUIRED)
find_package(Qt5Svg 5.3 REQUIRED)
find_package(Qt5Test 5.3 REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# The list of source and header files
file(GLOB_RECURSE SRCS ./src/*.cpp)
file(GLOB_RECURSE HDRS ./include/*.h)

# Setting the directory with headers
include_directories(include)

# Adding the file with resources
qt5_add_resources(SRCS resources.qrc)

# Tell CMake to create the helloworld executable
add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} Qt5::Qml Qt5::Gui Qt5::Quick Qt5::SerialPort Qt5::Test)
