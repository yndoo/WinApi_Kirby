#pragma once
#include <EngineCore/Level.h>
#include <EnginePlatform\EngineSound.h>
// Ό³Έν :
class UTitleLevel : public ULevel
{
public:
	// constrcuter destructer
	UTitleLevel();
	~UTitleLevel();

	// delete Function
	UTitleLevel(const UTitleLevel& _Other) = delete;
	UTitleLevel(UTitleLevel&& _Other) noexcept = delete;
	UTitleLevel& operator=(const UTitleLevel& _Other) = delete;
	UTitleLevel& operator=(UTitleLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private:
	UImageRenderer* Renderer = nullptr;
	UEngineSoundPlayer bgm;

	float AlphaTime = 0.0f;
	bool Dir = false;
};

