#pragma once
#include <EngineCore/Level.h>
#include "Player.h"
#include "Map.h"

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
	AMap* Map = nullptr;

	FVector MapSize = FVector::Zero;
	FVector WinScale = FVector::Zero;
};

