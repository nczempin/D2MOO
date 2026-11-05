
D2MOO_prepare_targets(D2Client)

target_link_libraries(${D2ClientImplName}
  PUBLIC
    D2CommonDefinitions
  PRIVATE
    Fog
    Storm
    D2CMP
    D2Common
    D2Game
    D2Gfx
    D2Lang
    D2MCPClient
    D2Net
    D2Sound
    D2Win
)

target_sources(${D2ClientImplName}
  PRIVATE
    src/D2Client.cpp

    include/D2Client.h
)

D2MOO_target_source_group(D2Client)
