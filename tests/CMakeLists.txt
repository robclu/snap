# ---- snap/tests/CMakeLists.txt -------------------------------------------- #

IF(WIN32)
  SET(BOOST_ROOT C:/Program Files/boost)
  SET(BOOST_LIBRARYDIR C:/Program Files/boost/lib)
  SET(Boost_USE_STATIC_LIBS ON)
ELSEIF(APPLE)
  SET(BOOST_ROOT /opt/boost)
  SET(BOOST_LIBRARYDIR /opt/boost/lib)
  SET(Boost_USE_STATIC_LIBS OFF)
ENDIF()

find_package(Boost COMPONENTS 
  system 
  filesystem 
  unit_test_framework 
  REQUIRED
)

link_directories(${Boost_LIBRARY_DIRS})

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

# ---- Make Test Function --------------------------------------------------- #

function(MakeTest TestName TestFiles TestLibs TestBinDir)
  add_executable(${${TestName}} ${${TestFiles}})

  target_link_libraries(${${TestName}} ${${TestLibs}})

  # Move test binary to the correct directory
  set_target_properties(${${TestName}} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${${TestBinDir}})

  add_test(NAME ${${TestName}} WORKING_DIRECTORY ${${TestBinDir}} COMMAND
    ${${TestBinDir}}/${${TestName}})
endfunction()

# ---- Config Tests --------------------------------------------------------- #

set(test_name config_tests)
set(test_files config_tests.cc)
set(test_libs 
  ${Boost_FILESYSTEM_LIBRARY} 
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
set(test_bin_dir ${Snap_SOURCE_DIR}/bin)

MakeTest(test_name test_files test_libs test_bin_dir)

# --------------------------------------------------------------------------- #
