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

set(TEST_BIN_DIR ${Snap_SOURCE_DIR}/bin/tests)
set(ASM_DIR ${Snap_SOURCE_DIR}/asm/tests)

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

function(MakeAsm AsmName AsmInputFiles AsmLibs AsmDir)
  add_executable(${${AsmName}} ${${AsmInputFiles}})

  target_link_libraries(${${AsmName}} ${${AsmLibs}})

  set_target_properties(${${AsmName}} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${${AsmDir}})

endfunction()

# ---- Config Tests --------------------------------------------------------- #

set(TEST_NAME config_tests)
set(TEST_FILES config_tests.cc)
set(TEST_LIBS
  ${Boost_FILESYSTEM_LIBRARY} 
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

MakeTest(TEST_NAME TEST_FILES TEST_LIBS TEST_BIN_DIR)

IF(GENERATE_ASM)
  set(ASM_NAME config_tests_asm)
  set(ASM_FILES config_tests.cc)
  set(ASM_LIBS ${TEST_LIBS})
  MakeAsm(ASM_NAME ASM_FILES ASM_LIBS ASM_DIR)
ENDIF()

# ---- Svec Tests ----------------------------------------------------------- #

set(TEST_NAME svec_tests)
set(TEST_FILES svec_tests.cc)
set(TEST_LIBS
  ${Boost_FILESYSTEM_LIBRARY} 
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

MakeTest(TEST_NAME TEST_FILES TEST_LIBS TEST_BIN_DIR)

IF(GENERATE_ASM)
  set(ASM_NAME svec_tests_asm)
  set(ASM_FILES svec_tests.cc)
  set(ASM_LIBS ${TEST_LIBS})
  MakeAsm(ASM_NAME ASM_FILES ASM_LIBS ASM_DIR)
ENDIF()

# ---- Utility Tests -------------------------------------------------------- #

set(TEST_NAME utility_tests)
set(TEST_FILES utility_tests.cc)
set(TEST_LIBS
  ${Boost_FILESYSTEM_LIBRARY} 
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

MakeTest(TEST_NAME TEST_FILES TEST_LIBS TEST_BIN_DIR)

IF(GENERATE_ASM)
  set(ASM_NAME utility_tests_asm)
  set(ASM_FILES utility_tests.cc)
  set(ASM_LIBS ${TEST_LIBS})
  MakeAsm(ASM_NAME ASM_FILES ASM_LIBS ASM_DIR)
ENDIF()

# --------------------------------------------------------------------------- #
