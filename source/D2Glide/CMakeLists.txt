D2MOO_prepare_targets(D2Glide)

target_link_libraries(${D2GlideImplName} PRIVATE Fog Storm D2CMP D2Gfx D2CommonDefinitions opengl32 gdi32 user32)

target_sources(${D2GlideImplName}
  PRIVATE
    src/D2Glide.cpp
    include/D2Glide.h
)

D2MOO_target_source_group(D2Glide)
