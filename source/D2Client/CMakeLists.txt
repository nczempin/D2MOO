
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
    src/reconstructed/DRAW_dLightMap.cpp
    src/reconstructed/ENGINE_Gfx.cpp
    src/reconstructed/GAME_Game.cpp
    src/reconstructed/GAME_Roster.cpp
    src/reconstructed/GAME_Wall2.cpp
    src/reconstructed/Msg.cpp
    src/reconstructed/SCmd.cpp
    src/reconstructed/Select.cpp
    src/reconstructed/SKILLS_Skills.cpp
    src/reconstructed/Sound_SoundHdr.cpp
    src/reconstructed/UI_automap.cpp
    src/reconstructed/UI_CmdTbl.cpp
    src/reconstructed/UI_Hireables.cpp
    src/reconstructed/UI_inv.cpp
    src/reconstructed/UI_npcmenu.cpp
    src/reconstructed/UI_QuestLog.cpp
    src/reconstructed/UI_showitems.cpp
    src/reconstructed/UI_SkillDesc.cpp
    src/reconstructed/UI_text.cpp
    src/reconstructed/UI_ui.cpp
    src/reconstructed/UI_useitem.cpp
    src/reconstructed/UNIT_CUnit.cpp
    src/reconstructed/UNIT_Item.cpp
    src/reconstructed/UNIT_Monster.cpp
    src/reconstructed/UNIT_Object.cpp
    src/reconstructed/UNIT_Player.cpp
    src/reconstructed/UNIT_PlrSkills.cpp
    src/reconstructed/UNIT_UnitMode.cpp
    src/reconstructed/View.cpp
    src/reconstructed/D2ClientLinkBridge.cpp

    include/D2Client.h
)

D2MOO_target_source_group(D2Client)
