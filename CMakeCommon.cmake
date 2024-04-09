# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Include directories
include_directories(${CMAKE_JKRGUI_DIR}/application)
include_directories(${CMAKE_JKRGUI_DIR}/jkrgui)
include_directories(${CMAKE_JKRGUI_DIR}/ksaivulkan)
include_directories(${CMAKE_JKRGUI_DIR}/ksaivulkan/Vulkan)
include_directories(${CMAKE_JKRGUI_DIR}/ksaivulkan/Vendor)
include_directories(${CMAKE_JKRGUI_DIR}/ksaivulkan/Vendor/stbi)
#include_directories(${SPIRV-Headers_SOURCE_DIR}/include})
include_directories(${CMAKE_JKRGUI_DIR}/libs/)
include_directories(${CMAKE_JKRGUI_DIR}/aJkrLua)
include_directories(${CMAKE_JKRGUI_DIR}/vendor/lua)

# If building for Android, set specific compiler flags
if(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wl,--exclude-libs,ALL")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif()

# Define debug flags for Android builds
if(ANDROID)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-D_DEBUG)
        set(NDK_DEBUG true)
    endif()
endif()

set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")

# Set include and library paths based on the platform
if(APPLE)
    set(VULKAN_INCLUDE_PATH "${VULKAN_SDK}/${VULKAN_VERSION}/macOS/include/")
    set(VULKAN_LIBRARY_PATH "${VULKAN_SDK}/${VULKAN_VERSION}/macOS/lib/")
else()
    set(VULKAN_INCLUDE_PATH "${VULKAN_SDK}/${VULKAN_VERSION}/Include/")
    set(VULKAN_LIBRARY_PATH "${VULKAN_SDK}/${VULKAN_VERSION}/Lib/")
endif()

# Set the number of parallel builds
set(CMAKE_BUILD_PARALLEL_LEVEL 8)

# Define additional flags based on the platform and build type
if(ANDROID OR APPLE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SDL_MAIN_HANDLED)
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SDL_MAIN_HANDLED -DTRACY_ENABLE)
    else()
        add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SDL_MAIN_HANDLED)
    endif()
endif()

# Set debug information format for MSVC
if(MSVC)
    if(POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

# Add platform-specific definitions
if(ANDROID)
    add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)
    add_definitions(-DANDROID)
    add_definitions(-DVULKAN_HPP_NO_SMART_HANDLE)
    set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
    include_directories(${APP_GLUE_DIR})
elseif(WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
endif()

if(APPLE)
elseif(ANDROID)
    include_directories("${CMAKE_JKRGUI_DIR}/libs/AndroidInclude/SDL2")
else()
    include_directories(ksaivulkan/Vendor/Tracy/tracy)
endif()

set(Vulkan_LIBRARIES "vulkan-1")
include_directories(${VULKAN_INCLUDE_PATH})
# Link directories based on platform
if(ANDROID)
    link_directories("${CMAKE_JKRGUI_DIR}/libs/Android/")
else()
    link_directories(${VULKAN_LIBRARY_PATH})
    link_directories("${CMAKE_JKRGUI_DIR}/libs/")
endif()

# Additional link directories for Apple
if(APPLE)
    link_directories("/opt/homebrew/lib/")
    link_directories("/usr/local/lib/")
endif()

# Define a function to set up the target based on the build type
if(APPLE)
function(configure_target TARGET_NAME)
        target_link_libraries(${TARGET_NAME}
            ${Vulkan_LIBRARIES}
            SDL2
            SPIRV
            MachineIndependent
            SPIRV-Tools
            SPIRV-Tools-diff
            SPIRV-Tools-link
            SPIRV-Tools-lint
            SPIRV-Tools-opt
            SPIRV-Tools-reduce
            SPIRV-Tools-shared
            OSDependent
            GenericCodeGen
            glslang-default-resource-limits
            spirv-cross-c-shared
            spirv-cross-c
            spirv-cross-core
            spirv-cross-cpp
            spirv-cross-glsl
            spirv-cross-hlsl
            spirv-cross-msl
            spirv-cross-reflect
            lua
            freetype
            harfbuzz
            harfbuzz-subset
        )
endfunction()

elseif(ANDROID)

function(configure_target TARGET_NAME)
   target_link_libraries(${TARGET_NAME}
	  ${Vulkan_LIBRARIES}
	  android
	  log
	  brotlicommon-static
		brotlidec-static
		brotlienc-static
		bz2
		freetype
		GenericCodeGen
		glslang-default-resource-limits
		glslang
		harfbuzz-subset
		harfbuzz
		lua
		MachineIndependent
		OSDependent
		png
		png16
		SDL2
		spirv-cross-c
		spirv-cross-core
		spirv-cross-cpp
		spirv-cross-glsl
		spirv-cross-hlsl
		spirv-cross-msl
		spirv-cross-reflect
		spirv-cross-util
		SPIRV-Tools-diff
		SPIRV-Tools-link
		SPIRV-Tools-lint
		SPIRV-Tools-opt
		SPIRV-Tools-reduce
		SPIRV-Tools
		SPIRV
		SPVRemapper
		z
   )
endfunction()


else()


function(configure_target TARGET_NAME)
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        target_link_libraries(${TARGET_NAME}
            ${Vulkan_LIBRARIES}
            SDL2
            SPIRV
            MachineIndependent
            SPIRV-Tools
            SPIRV-Tools-diff
            SPIRV-Tools-link
            SPIRV-Tools-lint
            SPIRV-Tools-opt
            SPIRV-Tools-reduce
            SPIRV-Tools-shared
            OSDependent
            GenericCodeGen
            glslang-default-resource-limits
            spirv-cross-c-shared
            spirv-cross-c
            spirv-cross-core
            spirv-cross-cpp
            spirv-cross-glsl
            spirv-cross-hlsl
            spirv-cross-msl
            spirv-cross-reflect
            lua
            freetype
            harfbuzz
            harfbuzz-subset
            ws2_32
            wsock32
            dbghelp
        )
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(${TARGET_NAME}
            ${Vulkan_LIBRARIES}
            SDL2
            SPIRVd
            MachineIndependentd
            SPIRV-Toolsd
            SPIRV-Tools-diffd
            SPIRV-Tools-linkd
            SPIRV-Tools-lintd
            SPIRV-Tools-optd
            SPIRV-Tools-reduced
            SPIRV-Tools-sharedd
            OSDependentd
            GenericCodeGend
            glslang-default-resource-limitsd
            spirv-cross-c-sharedd
            spirv-cross-cd
            spirv-cross-cored
            spirv-cross-cppd
            spirv-cross-glsld
            spirv-cross-hlsld
            spirv-cross-msld
            spirv-cross-reflectd
            lua
            freetype
            harfbuzz
            harfbuzz-subset
            ws2_32
            wsock32
            dbghelp
        )
    endif()
endfunction()

endif()





if(APPLE)
elseif(DONTDOTHIS)
function(configure_llvm TARGET_NAME)
        target_link_libraries(${TARGET_NAME}
            LLVMWindowsManifest
            LLVMXRay
            LLVMLibDriver
            LLVMDlltoolDriver
            LLVMCoverage
            LLVMLineEditor
            LLVMXCoreDisassembler
            LLVMXCoreCodeGen
            LLVMXCoreDesc
            LLVMXCoreInfo
            LLVMX86TargetMCA
            LLVMX86Disassembler
            LLVMX86AsmParser
            LLVMX86CodeGen
            LLVMX86Desc
            LLVMX86Info
            LLVMWebAssemblyDisassembler
            LLVMWebAssemblyAsmParser
            LLVMWebAssemblyCodeGen
            LLVMWebAssemblyUtils
            LLVMWebAssemblyDesc
            LLVMWebAssemblyInfo
            LLVMVEDisassembler
            LLVMVEAsmParser
            LLVMVECodeGen
            LLVMVEDesc
            LLVMVEInfo
            LLVMSystemZDisassembler
            LLVMSystemZAsmParser
            LLVMSystemZCodeGen
            LLVMSystemZDesc
            LLVMSystemZInfo
            LLVMSparcDisassembler
            LLVMSparcAsmParser
            LLVMSparcCodeGen
            LLVMSparcDesc
            LLVMSparcInfo
            LLVMRISCVTargetMCA
            LLVMRISCVDisassembler
            LLVMRISCVAsmParser
            LLVMRISCVCodeGen
            LLVMRISCVDesc
            LLVMRISCVInfo
            LLVMPowerPCDisassembler
            LLVMPowerPCAsmParser
            LLVMPowerPCCodeGen
            LLVMPowerPCDesc
            LLVMPowerPCInfo
            LLVMNVPTXCodeGen
            LLVMNVPTXDesc
            LLVMNVPTXInfo
            LLVMMSP430Disassembler
            LLVMMSP430AsmParser
            LLVMMSP430CodeGen
            LLVMMSP430Desc
            LLVMMSP430Info
            LLVMMipsDisassembler
            LLVMMipsAsmParser
            LLVMMipsCodeGen
            LLVMMipsDesc
            LLVMMipsInfo
            LLVMLoongArchDisassembler
            LLVMLoongArchAsmParser
            LLVMLoongArchCodeGen
            LLVMLoongArchDesc
            LLVMLoongArchInfo
            LLVMLanaiDisassembler
            LLVMLanaiCodeGen
            LLVMLanaiAsmParser
            LLVMLanaiDesc
            LLVMLanaiInfo
            LLVMHexagonDisassembler
            LLVMHexagonCodeGen
            LLVMHexagonAsmParser
            LLVMHexagonDesc
            LLVMHexagonInfo
            LLVMBPFDisassembler
            LLVMBPFAsmParser
            LLVMBPFCodeGen
            LLVMBPFDesc
            LLVMBPFInfo
            LLVMAVRDisassembler
            LLVMAVRAsmParser
            LLVMAVRCodeGen
            LLVMAVRDesc
            LLVMAVRInfo
            LLVMARMDisassembler
            LLVMARMAsmParser
            LLVMARMCodeGen
            LLVMARMDesc
            LLVMARMUtils
            LLVMARMInfo
            LLVMAMDGPUTargetMCA
            LLVMAMDGPUDisassembler
            LLVMAMDGPUAsmParser
            LLVMAMDGPUCodeGen
            LLVMAMDGPUDesc
            LLVMAMDGPUUtils
            LLVMAMDGPUInfo
            LLVMAArch64Disassembler
            LLVMAArch64AsmParser
            LLVMAArch64CodeGen
            LLVMAArch64Desc
            LLVMAArch64Utils
            LLVMAArch64Info
            LLVMOrcJIT
            LLVMWindowsDriver
            LLVMMCJIT
            LLVMJITLink
            LLVMInterpreter
            LLVMExecutionEngine
            LLVMRuntimeDyld
            LLVMOrcTargetProcess
            LLVMOrcShared
            LLVMDWP
            LLVMDebugInfoLogicalView
            LLVMDebugInfoGSYM
            LLVMOption
            LLVMObjectYAML
            LLVMObjCopy
            LLVMMCA
            LLVMMCDisassembler
            LLVMLTO
            LLVMPasses
            LLVMCFGuard
            LLVMCoroutines
            LLVMipo
            LLVMVectorize
            LLVMLinker
            LLVMInstrumentation
            LLVMFrontendOpenMP
            LLVMFrontendOpenACC
            LLVMFrontendHLSL
            LLVMExtensions
            LLVMDWARFLinkerParallel
            LLVMDWARFLinker
            LLVMGlobalISel
            LLVMMIRParser
            LLVMAsmPrinter
            LLVMSelectionDAG
            LLVMCodeGen
            LLVMTarget
            LLVMObjCARCOpts
            LLVMCodeGenTypes
            LLVMIRPrinter
            LLVMInterfaceStub
            LLVMFileCheck
            LLVMFuzzMutate
            LLVMScalarOpts
            LLVMInstCombine
            LLVMAggressiveInstCombine
            LLVMTransformUtils
            LLVMBitWriter
            LLVMAnalysis
            LLVMProfileData
            LLVMSymbolize
            LLVMDebugInfoBTF
            LLVMDebugInfoPDB
            LLVMDebugInfoMSF
            LLVMDebugInfoDWARF
            LLVMObject
            LLVMTextAPI
            LLVMMCParser
            LLVMIRReader
            LLVMAsmParser
            LLVMMC
            LLVMDebugInfoCodeView
            LLVMBitReader
            LLVMFuzzerCLI
            LLVMCore
            LLVMRemarks
            LLVMBitstreamReader
            LLVMBinaryFormat
            LLVMTargetParser
            LLVMTableGen
            LLVMSupport
            LLVMDemangle
        )
endfunction()
endif()


function(copyToDestination cpyName)
  if(ANDROID)
    # add_custom_command(TARGET ${cpyName} POST_BUILD
    #   COMMAND ${CMAKE_COMMAND} -E copy
    #   ${CMAKE_CURRENT_BINARY_DIR}/lib${cpyName}.so
    #   ${CMAKE_JKRGUI_DIR}/libs/Android/lib${cpyName}.so
    # )
    # add_custom_command(TARGET ${cpyName} POST_BUILD
    #   COMMAND ${CMAKE_COMMAND} -E copy
    #   ${CMAKE_CURRENT_BINARY_DIR}/lib${cpyName}.so
    #   ${CMAKE_JKRGUI_DIR}/SDL4droid/aJkrLua_app/app/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/lib${cpyName}.so
    # )

    # add_custom_command(TARGET ${cpyName} POST_BUILD
    #   COMMAND ${CMAKE_COMMAND} -E copy
    #   ${CMAKE_JKRGUI_DIR}/libs/Android/liblua.so
    #   ${CMAKE_JKRGUI_DIR}/SDL4droid/aJkrLua_app/app/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/liblua.so
    # )
  endif()
endfunction()

function(someFlags inName)
     set_property(TARGET ${inName} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endfunction()
