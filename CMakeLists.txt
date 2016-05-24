# ---- snap/CmakeLists.txt -------------------------------------------------- #

cmake_minimum_required(VERSION 3.1)
project(Snap)

# ---- Versioning ----------------------------------------------------------- #

set(SNAP_MAJOR_VERSION 0)
set(SNAP_MINOR_VERSION 1)
set(SNAP_PATCH_VERSION 0)
set(SNAP_VERSION
  ${SNAP_MAJOR_VERSION}.${SNAP_MINOR_VERSION}.${SNAP_PATCH_VERSION}
)

# ---- Build Type ----------------------------------------------------------- #

IF(NOT CMAKE_BUILD_TYPE) 
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type not specified!")
ENDIF(NOT CMAKE_BUILD_TYPE)

# ---- Include directories -------------------------------------------------- #

include_directories(${Snap_SOURCE_DIR}/include)

# ---- Subdirectories ------------------------------------------------------- #

add_subdirectory(tests)

# ---- Compiler Flags ------------------------------------------------------- #

IF(WIN32)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /O2")
ELSE()
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14"     )
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pedantic -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14"     )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall")
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES Release)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}" 
    CACHE STRING "CXX FLAGS" FORCE)
ELSE()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}" 
    CACHE STRING "CXX FLAGS" FORCE)
ENDIF()

# ---- Num CPUS ------------------------------------------------------------- #

include(ProcessorCount)
ProcessorCount(PROC_COUNT)

IF(PROC_COUNT EQUAL 0)
  set(PROC_COUNT 0)

  # Linux:
  set(CPUINFO_FILE "/proc/cpuinfo")
  IF(EXISTS "${CPUINFO_FILE}")
    file(STRINGS "${CPUINFO_FILE}" procs REGEX "^processor.: [0-9]+$")
    list(LENGTH procs PROC_COUNT)
  ENDIF()

  # Mac:
  IF(APPLE)
    find_program(CMD_SYS_CTL "sysctl -a | grep machdep.cpu")
    IF(CMD_SYS_CTL)
      execute_process(COMMAND ${CMD_SYS_PRO} OUTPUT_VARIABLE INFO)
      message("CPUINFO: ${INFO}")
      string(REGEX REPLACE "^.*Total Number Of Cores: ([0-9]+).*$" "\\1"
        PROC_COUNT "${INFO}")
    ENDIF()
  ENDIF()

  # Windows:
  IF(WIN32)
    set(PROC_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
  ENDIF()
ENDIF()

IF(PROC_COUNT)
  set(CTEST_BUILD_FLAGS "-j${PROC_COUNT}")
ENDIF()

# ---- Vectorized Intrinsics ------------------------------------------------ #

# APPLE INTEL INTRINSICS:
IF(APPLE)
  EXEC_PROGRAM(sysctl ARGS "-a | grep machdep.cpu" OUTPUT_VARIABLE CPUINFO)

  STRING(REGEX MATCH "SSE" SSE_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "SSE" "${SSE_THERE}" SSE_TRUE)

  STRING(REGEX MATCH "SSE2" SSE2_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "SSE2" "${SSE2_THERE}" SSE2_TRUE)

  STRING(REGEX MATCH "SSSE3" SSE3_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "SSSE3" "${SSE3_THERE}" SSSE3_TRUE)

  STRING(REGEX MATCH "SSE4.1" SSE41_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "SSE4.1" "${SSE41_THERE}" SSE41_TRUE)

  STRING(REGEX MATCH "SSE4.2" SSE42_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "SSE4.2" "${SSE42_THERE}" SSE42_TRUE)

  STRING(REGEX MATCH "AVX1.0" AVX_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "AVX1.0" "${AVX_THERE}" AVX_TRUE)

  STRING(REGEX MATCH "AVX2.0" AVX2_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "AVX2.0" "${AVX2_THERE}" AVX2_TRUE)

  message("SSE: ${SSE42_TRUE}, AVX ${AVX_TRUE}")
ENDIF(APPLE)

# LINUX INTEL INTRINCICS:
IF(UNIX AND NOT APPLE)
  EXEC_PROGRAM(cat ARGS "/proc/cpuinfo" OUTPUT_VARIABLE CPUINFO)

  STRING(REGEX MATCH "sse" SSE_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "sse" "${SSE_THERE}" SSE_TRUE)

  STRING(REGEX MATCH "sse2" SSE2_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "sse2" "${SSE2_THERE}" SSE2_TRUE)

  # /proc/cpuinfo apparently omits sse3 :( bad luck atm!

  STRING(REGEX MATCH "ssse3" SSE3_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "ssse3" "${SSE3_THERE}" SSSE3_TRUE)

  STRING(REGEX MATCH "sse4_1" SSE41_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "sse4_1" "${SSE41_THERE}" SSE41_TRUE)

  STRING(REGEX MATCH "sse4_2" SSE42_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "sse4_2" "${SSE42_THERE}" SSE42_TRUE)

  STRING(REGEX MATCH "avx" AVX_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "avx" "${AVX_THERE}" AVX_TRUE)

  STRING(REGEX MATCH "avx2" AVX2_THERE ${CPUINFO})
  STRING(COMPARE EQUAL "avx2" "${AVX2_THERE}" AVX2_TRUE)
ENDIF(UNIX)

  message("SSE: ${SSE42_TRUE}, AVX ${AVX_TRUE}")

IF(AVX2_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx2")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -mavx2"  )
ELSEIF(AVX_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -mavx"  )
ELSEIF(SSE42_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse4.2")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -msse4.2"  )
ELSEIF(SSE41_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse4.1")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -msse4.1"  )
ELSEIF(SSSE3_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mssse3")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -mssse3"  )
ELSEIF(SSE3_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse3")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -msse3"  )
ELSEIF(SSE2_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse2")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -msse2")
ELSEIF(SSE_TRUE)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -msse"  )
ELSE(AVX2_TRUE)
ENDIF(AVX2_TRUE)

# ARM
IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=softfp")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=neon -Otime" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -vectorize"        )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=softfp")
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=neon -Otime" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -vectorize"        )
ENDIF()

# ---- Tests ---------------------------------------------------------------- #

enable_testing()

add_test(NAME config_tests COMMAND config_tests)

set(TESTS_STRING "config")

# ---- Boost ---------------------------------------------------------------- #

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

set(BOOST_VERSION_HR "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}")
set(BOOST_VERSION_HR "${BOOST_VERSION_HR}.${Boost_SUBMINOR_VERSION}")

# ---- Print Configuration -------------------------------------------------- #

message("# ---- CONFIGURATION --------------------------------------------- #")
message("| SNAP VERSION            : ${SNAP_VERSION}"                         )
message("| BUILD TYPE              : ${CMAKE_BUILD_TYPE}"                     )
message("| CMAKE_CXX_FLAGS_RELEASE : ${CMAKE_CXX_FLAGS_RELEASE}"              )
message("| CMAKE_CXX_FLAGS_DEBUG   : ${CMAKE_CXX_FLAGS_DEBUG}"                )
message("| CMAKE_CXX_FLAGS         : ${CMAKE_CXX_FLAGS}"                      )
message("| NUMBER OF PROCESSORS    : ${PROC_COUNT}"                           )
message("| BOOST VERSION           : ${BOOST_VERSION_HR}"                     )
message("| TESTS                   : ${TESTS_STRING}"                         )
message("# ---------------------------------------------------------------- #")

