#include <Drlg/D2DrlgDrlg.h>
#include <cstddef>

static_assert(sizeof(D2DrlgActStrc) == 0x60, "D2DrlgActStrc size must be 0x60");
static_assert(sizeof(D2ActiveRoomStrc) == 0x80, "D2ActiveRoomStrc size must be 0x80");
static_assert(sizeof(D2DrlgRoomStrc) == 0xEC, "D2DrlgRoomStrc size must be 0xEC");
static_assert(sizeof(D2DrlgStrc) == 0x488, "D2DrlgStrc size must be 0x488");
static_assert(sizeof(D2DrlgLevelStrc) == 0x230, "D2DrlgLevelStrc size must be 0x230");

#if defined(D2_VERSION_112A)
static_assert(offsetof(D2DrlgActStrc, dwInitSeed) == 0x00, "D2DrlgActStrc::dwInitSeed offset");
static_assert(offsetof(D2DrlgActStrc, pTileData) == 0x04, "D2DrlgActStrc::pTileData offset");
static_assert(offsetof(D2DrlgActStrc, pRoom) == 0x34, "D2DrlgActStrc::pRoom offset");
static_assert(offsetof(D2DrlgActStrc, pDrlg) == 0x38, "D2DrlgActStrc::pDrlg offset");
static_assert(offsetof(D2DrlgActStrc, nAct) == 0x44, "D2DrlgActStrc::nAct offset");
static_assert(offsetof(D2DrlgActStrc, pMemPool) == 0x48, "D2DrlgActStrc::pMemPool offset");
static_assert(offsetof(D2DrlgActStrc, bHasPendingRoomsUpdates) == 0x5C, "D2DrlgActStrc::bHasPendingRoomsUpdates offset");

static_assert(offsetof(D2ActiveRoomStrc, ppRoomList) == 0x00, "D2ActiveRoomStrc::ppRoomList offset");
static_assert(offsetof(D2ActiveRoomStrc, pAct) == 0x04, "D2ActiveRoomStrc::pAct offset");
static_assert(offsetof(D2ActiveRoomStrc, pSeed) == 0x0C, "D2ActiveRoomStrc::pSeed offset");
static_assert(offsetof(D2ActiveRoomStrc, tCoords) == 0x18, "D2ActiveRoomStrc::tCoords offset");
static_assert(offsetof(D2ActiveRoomStrc, pRoomNext) == 0x38, "D2ActiveRoomStrc::pRoomNext offset");
static_assert(offsetof(D2ActiveRoomStrc, dwFlags) == 0x3C, "D2ActiveRoomStrc::dwFlags offset");
static_assert(offsetof(D2ActiveRoomStrc, pUnitFirst) == 0x40, "D2ActiveRoomStrc::pUnitFirst offset");
static_assert(offsetof(D2ActiveRoomStrc, pCollisionGrid) == 0x50, "D2ActiveRoomStrc::pCollisionGrid offset");
static_assert(offsetof(D2ActiveRoomStrc, pDrlgRoom) == 0x70, "D2ActiveRoomStrc::pDrlgRoom offset");
static_assert(offsetof(D2ActiveRoomStrc, ppClients) == 0x7C, "D2ActiveRoomStrc::ppClients offset");

static_assert(offsetof(D2DrlgRoomStrc, pLevel) == 0x00, "D2DrlgRoomStrc::pLevel offset");
static_assert(offsetof(D2DrlgRoomStrc, pDrlgOrth) == 0x04, "D2DrlgRoomStrc::pDrlgOrth offset");
static_assert(offsetof(D2DrlgRoomStrc, pStatusNext) == 0x28, "D2DrlgRoomStrc::pStatusNext offset");
static_assert(offsetof(D2DrlgRoomStrc, pDrlgCoord) == 0x2C, "D2DrlgRoomStrc::pDrlgCoord offset");
static_assert(offsetof(D2DrlgRoomStrc, pTiles) == 0x40, "D2DrlgRoomStrc::pTiles offset");
static_assert(offsetof(D2DrlgRoomStrc, nType) == 0xC0, "D2DrlgRoomStrc::nType offset");
static_assert(offsetof(D2DrlgRoomStrc, fRoomStatus) == 0xC8, "D2DrlgRoomStrc::fRoomStatus offset");
static_assert(offsetof(D2DrlgRoomStrc, pTileGrid) == 0xD0, "D2DrlgRoomStrc::pTileGrid offset");
static_assert(offsetof(D2DrlgRoomStrc, pDrlgRoomNext) == 0xD4, "D2DrlgRoomStrc::pDrlgRoomNext offset");
static_assert(offsetof(D2DrlgRoomStrc, pRoom) == 0xD8, "D2DrlgRoomStrc::pRoom offset");
static_assert(offsetof(D2DrlgRoomStrc, pSeed) == 0xDC, "D2DrlgRoomStrc::pSeed offset");
static_assert(offsetof(D2DrlgRoomStrc, dwFlags) == 0xE4, "D2DrlgRoomStrc::dwFlags offset");
static_assert(offsetof(D2DrlgRoomStrc, pStatusPrev) == 0xE8, "D2DrlgRoomStrc::pStatusPrev offset");

static_assert(offsetof(D2DrlgStrc, pWarp) == 0x000, "D2DrlgStrc::pWarp offset");
static_assert(offsetof(D2DrlgStrc, pAct) == 0x004, "D2DrlgStrc::pAct offset");
static_assert(offsetof(D2DrlgStrc, tStatusRoomsLists) == 0x00C, "D2DrlgStrc::tStatusRoomsLists offset");
static_assert(offsetof(D2DrlgStrc, pfAutomap) == 0x3BC, "D2DrlgStrc::pfAutomap offset");
static_assert(offsetof(D2DrlgStrc, pTiles) == 0x3CC, "D2DrlgStrc::pTiles offset");
static_assert(offsetof(D2DrlgStrc, pMempool) == 0x45C, "D2DrlgStrc::pMempool offset");
static_assert(offsetof(D2DrlgStrc, pSeed) == 0x468, "D2DrlgStrc::pSeed offset");
static_assert(offsetof(D2DrlgStrc, pLevel) == 0x470, "D2DrlgStrc::pLevel offset");
static_assert(offsetof(D2DrlgStrc, pDrlgRoom) == 0x480, "D2DrlgStrc::pDrlgRoom offset");
static_assert(offsetof(D2DrlgStrc, nAllocatedRooms) == 0x484, "D2DrlgStrc::nAllocatedRooms offset");

static_assert(offsetof(D2DrlgLevelStrc, dwFlags) == 0x000, "D2DrlgLevelStrc::dwFlags offset");
static_assert(offsetof(D2DrlgLevelStrc, nDrlgType) == 0x050, "D2DrlgLevelStrc::nDrlgType offset");
static_assert(offsetof(D2DrlgLevelStrc, pSeed) == 0x054, "D2DrlgLevelStrc::pSeed offset");
static_assert(offsetof(D2DrlgLevelStrc, pNextLevel) == 0x05C, "D2DrlgLevelStrc::pNextLevel offset");
static_assert(offsetof(D2DrlgLevelStrc, pDrlg) == 0x064, "D2DrlgLevelStrc::pDrlg offset");
static_assert(offsetof(D2DrlgLevelStrc, pLevelCoords) == 0x06C, "D2DrlgLevelStrc::pLevelCoords offset");
static_assert(offsetof(D2DrlgLevelStrc, nCoordLists) == 0x07C, "D2DrlgLevelStrc::nCoordLists offset");
static_assert(offsetof(D2DrlgLevelStrc, dwInitSeed) == 0x08C, "D2DrlgLevelStrc::dwInitSeed offset");
static_assert(offsetof(D2DrlgLevelStrc, nJungleDefs) == 0x090, "D2DrlgLevelStrc::nJungleDefs offset");
static_assert(offsetof(D2DrlgLevelStrc, nLevelId) == 0x094, "D2DrlgLevelStrc::nLevelId offset");
static_assert(offsetof(D2DrlgLevelStrc, pTileInfo) == 0x09C, "D2DrlgLevelStrc::pTileInfo offset");
static_assert(offsetof(D2DrlgLevelStrc, pFirstRoomEx) == 0x21C, "D2DrlgLevelStrc::pFirstRoomEx offset");
static_assert(offsetof(D2DrlgLevelStrc, pPreset) == 0x22C, "D2DrlgLevelStrc::pPreset offset");
#endif
