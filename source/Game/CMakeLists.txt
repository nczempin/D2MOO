
add_executable(Game WIN32 
  src/Main.cpp
  include/Main.h
  
  resources/Game.rc
)

if(EXISTS resources/Game.${D2MOO_ORDINALS_VERSION}.rc)
  target_sources(Game PRIVATE resources/Game.${D2MOO_ORDINALS_VERSION}.rc)
endif()

target_include_directories(Game PRIVATE include)
target_link_libraries(Game 
  PRIVATE
    Fog
	Storm
	D2Common
	D2Win
	D2Gfx
	D2Sound
	D2MCPClient
)
add_library(D2LaunchOrigShim SHARED
  src/OrigLaunchShim.cpp
  resources/OrigLaunchShim.def
)
target_link_libraries(D2LaunchOrigShim
  PRIVATE
    D2CommonDefinitions
    advapi32
)
set_target_properties(D2LaunchOrigShim PROPERTIES
  OUTPUT_NAME D2LaunchOrigShim
)

add_dependencies(Game
  D2Launch
  D2LaunchOrigShim
  D2Client
  D2Game
  D2Common
  D2Net
  D2Lang
  D2CMP
  D2Gdi
)
set_target_properties(Game PROPERTIES
  OUTPUT_NAME Game.D2MOO
  VS_DEBUGGER_COMMAND_ARGUMENTS "-w"
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
  VS_DEBUGGER_ENVIRONMENT "D2MOO_USE_TS_CLIENT=1;D2MOO_USE_ORIGINAL_CLIENT=0"
)

add_custom_command(TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Game>" "$<TARGET_FILE_DIR:Game>/Game-NativeClient.exe"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Game>" "$<TARGET_FILE_DIR:Game>/Game.exe"
  COMMENT "Syncing Game.D2MOO.exe to Game-NativeClient.exe and Game.exe"
)

add_custom_target(Game-TSClient DEPENDS Game)
set_target_properties(Game-TSClient PROPERTIES
  VS_DEBUGGER_COMMAND "$<TARGET_FILE:Game>"
  VS_DEBUGGER_COMMAND_ARGUMENTS "-w"
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
  VS_DEBUGGER_ENVIRONMENT "D2MOO_USE_TS_CLIENT=1;D2MOO_USE_ORIGINAL_CLIENT=0"
)

add_custom_target(Game-OriginalClient DEPENDS Game)
set_target_properties(Game-OriginalClient PROPERTIES
  VS_DEBUGGER_COMMAND "$<TARGET_FILE:Game>"
  VS_DEBUGGER_COMMAND_ARGUMENTS "-w"
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
  VS_DEBUGGER_ENVIRONMENT "D2MOO_USE_ORIGINAL_CLIENT=1;D2MOO_USE_TS_CLIENT=0"
)

add_custom_target(Game-NativeClient DEPENDS Game)
set_target_properties(Game-NativeClient PROPERTIES
  VS_DEBUGGER_COMMAND "$<TARGET_FILE:Game>"
  VS_DEBUGGER_COMMAND_ARGUMENTS "-w"
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
  VS_DEBUGGER_ENVIRONMENT "D2MOO_USE_NATIVE_CLIENT=1;D2MOO_USE_TS_CLIENT=0;D2MOO_USE_ORIGINAL_CLIENT=0"
)

if(D2MOO_WITH_STATIC_TESTS)
  target_sources(Game
    PRIVATE
      statictests/Game_staticchecks.cpp
  )
endif()
