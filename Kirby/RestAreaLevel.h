#pragma once
#include <EngineCore/Level.h>
#include "Player.h"
#include "Map.h"

// Ό³Έν :
class URestAreaLevel : public ULevel
{
public:
	// constrcuter destructer
	URestAreaLevel();
	~URestAreaLevel();

	// delete Function
	URestAreaLevel(const URestAreaLevel& _Other) = delete;
	URestAreaLevel(URestAreaLevel&& _Other) noexcept = delete;
	URestAreaLevel& operator=(const URestAreaLevel& _Other) = delete;
	URestAreaLevel& operator=(URestAreaLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private:
	APlayer* NewKirby = nullptr;
	AMap* Map = nullptr;
};

