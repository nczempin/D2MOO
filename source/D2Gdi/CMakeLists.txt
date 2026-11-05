D2MOO_prepare_targets(D2Gdi)

target_link_libraries(${D2GdiImplName} PRIVATE Fog Storm D2CMP D2Gfx D2CommonDefinitions)

target_sources(${D2GdiImplName}
  PRIVATE
    src/D2Gdi.cpp
    include/D2Gdi.h
)

D2MOO_target_source_group(D2Gdi)
