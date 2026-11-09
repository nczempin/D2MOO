#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <windows.h>
#include <cstdint>
#include <D2Seed.h>
#include <DataTbls/LevelsIds.h>
#include <Drlg/D2DrlgDrlg.h>

TEST_CASE("Fixed-Seed PRNG Determinism and 64-bit LCG Trajectory (0x112A2026)")
{
	D2SeedStrc tSeedA = {};
	D2SeedStrc tSeedB = {};

	SEED_InitSeed(&tSeedA);
	SEED_InitLowSeed(&tSeedA, static_cast<int>(0x112A2026u));

	SEED_InitSeed(&tSeedB);
	SEED_InitLowSeed(&tSeedB, static_cast<int>(0x112A2026u));

	CHECK(SEED_GetLowSeed(&tSeedA) == 0x112A2026u);
	CHECK(SEED_GetHighSeed(&tSeedA) == 666u);
	CHECK(tSeedA.lSeed == tSeedB.lSeed);

	for (int i = 0; i < 10000; ++i)
	{
		const uint64_t uRollA = SEED_RollRandomNumber(&tSeedA);
		const uint64_t uRollB = SEED_RollRandomNumber(&tSeedB);
		REQUIRE(uRollA == uRollB);
		REQUIRE(tSeedA.nLowSeed == tSeedB.nLowSeed);
		REQUIRE(tSeedA.nHighSeed == tSeedB.nHighSeed);
	}

	uint32_t dwFinalLow = 0;
	uint32_t dwFinalHigh = 0;
	SEED_GetSeeds(&tSeedA, &dwFinalLow, &dwFinalHigh);
	CHECK(dwFinalLow == tSeedA.nLowSeed);
	CHECK(dwFinalHigh == tSeedA.nHighSeed);

	// Verify SEED_SetSeeds / SEED_GetSeeds round-trip
	D2SeedStrc tRestored = {};
	SEED_SetSeeds(&tRestored, dwFinalLow, dwFinalHigh);
	CHECK(SEED_RollRandomNumber(&tRestored) == SEED_RollRandomNumber(&tSeedB));
}

TEST_CASE("Differential Oracle vs Retail 1.12a D2Common.dll SEED Ordinals")
{
	static const char* const kRetailCandidates[] = {
		"C:\\Users\\nicol\\.gemini\\antigravity\\worktrees\\rea-research-catalog\\d2moo_version_binaries_check\\targets\\diablo-ii-1.12a\\game\\D2Common.dll",
		"H:\\git\\D2MOO-112a\\targets\\diablo-ii-1.12a\\game\\D2Common.dll"
	};

	HMODULE hRetailD2Common = nullptr;
	for (const char* szPath : kRetailCandidates)
	{
		if (GetFileAttributesA(szPath) != INVALID_FILE_ATTRIBUTES)
		{
			hRetailD2Common = LoadLibraryExA(szPath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
			if (hRetailD2Common)
			{
				break;
			}
		}
	}

	if (hRetailD2Common)
	{
		using FnInitSeed = void(__fastcall*)(D2SeedStrc*);
		using FnInitLowSeed = void(__fastcall*)(D2SeedStrc*, int);
		using FnGetLowSeed = uint32_t(__fastcall*)(D2SeedStrc*);
		using FnGetHighSeed = uint32_t(__fastcall*)(D2SeedStrc*);
		using FnSetSeeds = void(__fastcall*)(D2SeedStrc*, uint32_t, uint32_t);
		using FnGetSeeds = void(__fastcall*)(D2SeedStrc*, uint32_t*, uint32_t*);

		const auto pfnRetailInitSeed = reinterpret_cast<FnInitSeed>(GetProcAddress(hRetailD2Common, MAKEINTRESOURCEA(11156)));
		const auto pfnRetailInitLowSeed = reinterpret_cast<FnInitLowSeed>(GetProcAddress(hRetailD2Common, MAKEINTRESOURCEA(10073)));
		const auto pfnRetailGetLowSeed = reinterpret_cast<FnGetLowSeed>(GetProcAddress(hRetailD2Common, MAKEINTRESOURCEA(10120)));
		const auto pfnRetailGetHighSeed = reinterpret_cast<FnGetHighSeed>(GetProcAddress(hRetailD2Common, MAKEINTRESOURCEA(10164)));
		const auto pfnRetailSetSeeds = reinterpret_cast<FnSetSeeds>(GetProcAddress(hRetailD2Common, MAKEINTRESOURCEA(10076)));
		const auto pfnRetailGetSeeds = reinterpret_cast<FnGetSeeds>(GetProcAddress(hRetailD2Common, MAKEINTRESOURCEA(10487)));

		REQUIRE(pfnRetailInitSeed != nullptr);
		REQUIRE(pfnRetailInitLowSeed != nullptr);
		REQUIRE(pfnRetailGetLowSeed != nullptr);
		REQUIRE(pfnRetailGetHighSeed != nullptr);
		REQUIRE(pfnRetailSetSeeds != nullptr);
		REQUIRE(pfnRetailGetSeeds != nullptr);

		static const uint32_t kTestSeeds[] = { 0x112A2026u, 0xDEADBEEFu, 0x00000001u, 0xFFFFFFFFu };
		for (uint32_t dwInit : kTestSeeds)
		{
			D2SeedStrc tNative = {};
			D2SeedStrc tRetail = {};

			SEED_InitSeed(&tNative);
			pfnRetailInitSeed(&tRetail);
			CHECK(tNative.lSeed == tRetail.lSeed);

			SEED_InitLowSeed(&tNative, static_cast<int>(dwInit));
			pfnRetailInitLowSeed(&tRetail, static_cast<int>(dwInit));
			CHECK(tNative.lSeed == tRetail.lSeed);
			CHECK(SEED_GetLowSeed(&tNative) == pfnRetailGetLowSeed(&tRetail));
			CHECK(SEED_GetHighSeed(&tNative) == pfnRetailGetHighSeed(&tRetail));

			pfnRetailSetSeeds(&tRetail, dwInit ^ 0x5A5A5A5Au, dwInit ^ 0xA5A5A5A5u);
			SEED_SetSeeds(&tNative, dwInit ^ 0x5A5A5A5Au, dwInit ^ 0xA5A5A5A5u);

			uint32_t nL = 0, nH = 0, rL = 0, rH = 0;
			SEED_GetSeeds(&tNative, &nL, &nH);
			pfnRetailGetSeeds(&tRetail, &rL, &rH);
			CHECK(nL == rL);
			CHECK(nH == rH);
		}

		FreeLibrary(hRetailD2Common);
	}
}

TEST_CASE("Acts I-V Level-to-Act and Town Level Classification Parity")
{
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_ROGUEENCAMPMENT) == 0);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_CATACOMBSLEV4) == 0);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_LUTGHOLEIN) == 1);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_DURIELSLAIR) == 1);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_KURASTDOCKTOWN) == 2);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_DURANCEOFHATELEV3) == 2);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_THEPANDEMONIUMFORTRESS) == 3);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_CHAOSSANCTUM) == 3);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_HARROGATH) == 4);
	CHECK(DRLG_GetActNoFromLevelId(LEVEL_THEWORLDSTONECHAMBER) == 4);

	CHECK(DRLG_IsTownLevel(LEVEL_ROGUEENCAMPMENT) == TRUE);
	CHECK(DRLG_IsTownLevel(LEVEL_BLOODMOOR) == FALSE);
	CHECK(DRLG_IsTownLevel(LEVEL_LUTGHOLEIN) == TRUE);
	CHECK(DRLG_IsTownLevel(LEVEL_DRYHILLS) == FALSE);
	CHECK(DRLG_IsTownLevel(LEVEL_KURASTDOCKTOWN) == TRUE);
	CHECK(DRLG_IsTownLevel(LEVEL_SPIDERFOREST) == FALSE);
	CHECK(DRLG_IsTownLevel(LEVEL_THEPANDEMONIUMFORTRESS) == TRUE);
	CHECK(DRLG_IsTownLevel(LEVEL_OUTERSTEPPES) == FALSE);
	CHECK(DRLG_IsTownLevel(LEVEL_HARROGATH) == TRUE);
	CHECK(DRLG_IsTownLevel(LEVEL_ID_ACT5_BARRICADE_1) == FALSE);
}
