#pragma once
#include <EngineCore/Level.h>
#include "Player.h"
#include "Map.h"
#include "Ladder.h"
#include "WoodBlock.h"
#include "Spawner.h"
#include "MrFrosty.h"
#include "DoorStar.h"
#include "WaddleDee.h"
#include "KirbyUI.h"

// Ό³Έν :
class UBossLevel : public ULevel
{
public:
	// constrcuter destructer
	UBossLevel();
	~UBossLevel();

	// delete Function
	UBossLevel(const UBossLevel& _Other) = delete;
	UBossLevel(UBossLevel&& _Other) noexcept = delete;
	UBossLevel& operator=(const UBossLevel& _Other) = delete;
	UBossLevel& operator=(UBossLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private:
	APlayer* NewKirby = nullptr;
	AMrFrosty* OneFrosty = nullptr;
	ASpawner* FrostySpawner = nullptr;
	ASpawner* FrostyStarter = nullptr;
	AMap* Map = nullptr;
	KirbyUI* UI = nullptr;

	//bool FrostySpawned = false;
	bool OpenDoorOnce = false;
	int KirbyHpData = 0;
	int KirbyLifeData = 0;

	FVector MapSize = FVector::Zero;
	FVector WinScale = FVector::Zero;
};

