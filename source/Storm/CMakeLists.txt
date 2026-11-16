
D2MOO_prepare_targets(Storm)

target_include_directories(${StormImplName} BEFORE PUBLIC include)
target_link_libraries(${StormImplName} PUBLIC D2CommonDefinitions squall::Storm)
target_compile_definitions(${StormImplName} PUBLIC WITH_SQUALL=1)

target_sources(${StormImplName}
  PRIVATE
    src/Storm.cpp
    src/StormMpq.cpp

    include/Storm.h
    include/StormHandles.h
    include/storm/Memory.hpp
    include/storm/Error.hpp
    include/storm/String.hpp
    include/storm/hash/TSHashTable.hpp
)

D2MOO_target_source_group(Storm)