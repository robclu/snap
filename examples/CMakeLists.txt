# ---- snap/examples/CMakeLists.txt ----------------------------------------- #

IF(NOT GENERATE_ASM)
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
ENDIF()

# ---- Output Directories --------------------------------------------------- #

set(EX_BIN_DIR ${Snap_SOURCE_DIR}/bin/examples)
set(ASM_DIR ${Snap_SOURCE_DIR}/asm/examples)

# ---- Functions ------------------------------------------------------------ #

# MAKE EXAMPLE
function(MAKE_EXAMPLE ExName ExFiles ExLibs ExBinDir)
  add_executable(${${ExName}} ${${ExFiles}})

  target_link_libraries(${${ExName}} ${${ExLibs}})

  set_target_properties(${${ExName}} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${${ExBinDir}})
endfunction()

# MAKE ASM
function(MAKE_ASM AsmName AsmInputFiles AsmLibs AsmDir)
  add_executable(${${AsmName}} ${${AsmInputFiles}})

  target_link_libraries(${${AsmName}} ${${AsmLibs}})

  set_target_properties(${${AsmName}} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${${AsmDir}})
endfunction()

# ---- Vec Examples --------------------------------------------------------- #

IF(GENERATE_ASM) 
  set(ASM_NAME svec_examples_asm)
  set(ASM_FILES svec_examples.cc)
  set(ASM_LIBS )
  MAKE_ASM(ASM_NAME ASM_FILES ASM_LIBS ASM_DIR)
ELSE(GENERATE_ASM)
  set(EX_NAME  svec_examples)
  set(EX_FILES svec_examples.cc)
  set(EX_LIBS
    ${Boost_FILESYSTEM_LIBRARY} 
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
  MAKE_EXAMPLE(EX_NAME EX_FILES EX_LIBS EX_BIN_DIR)
ENDIF(GENERATE_ASM)

# --------------------------------------------------------------------------- #
