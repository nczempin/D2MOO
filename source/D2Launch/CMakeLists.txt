
D2MOO_prepare_targets(D2Launch)

target_link_libraries(${D2LaunchImplName}
  PUBLIC
    D2CommonDefinitions
  PRIVATE
    Fog
    Storm
    D2Lang
    D2Win
    D2Gfx
    D2Sound
    D2Net
    D2MCPClient
)

target_sources(${D2LaunchImplName}
  PRIVATE
    src/D2Launch.cpp

    include/D2Launch.h
)

D2MOO_target_source_group(D2Launch)
