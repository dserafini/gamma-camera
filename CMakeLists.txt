# which is the minimum version of cmake that we need to compile our code
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# what is the name of the project that we want to start
project(Simulation)

# when you compile and source Geant4 then it automatically find that packages: Geant4
# Geant4 is required so without that you cannot run: REQUIRED
# we also need one package for user interface: ui_all
# and one for visualization: vis_all
find_package(Geant4 REQUIRED ui_all vis_all)
# when these packages are found then everything is working

# we also have to use the include files of Geant4 that have been set by sourcing
# so we include the files that are in the path ${Geant4_USE_FILE}
include(${Geant4_USE_FILE})

# in our project we will put different class in different files
# we want to compile all the source files (*.cc) that we have in our folder
# ${PROJECT_SOURCE_DIR} is where the CMakeLists.txt file is
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)

# same for the header files (*.hh)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

# copy all the macro files in the build folder
# but I have to cmake .. every change I make
# but I do not have to make
file(GLOB MACRO_FILES
  "*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

# we create the executable with a certain name: sim
# we have to write the main file where the source code is added: sim.cc
# we use the variables that we created: sources and headers
add_executable(sim sim.cc ${sources} ${headers})

# link to Geant4 libraries otherwise it cannot find them
target_link_libraries(sim ${Geant4_LIBRARIES})

# Adds a target with the given name that executes the given commands
add_custom_target(Simulation DEPENDS sim)

# copy data files into build folder
file(GLOB DATA_FILES
  "*.dat"
)
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})
