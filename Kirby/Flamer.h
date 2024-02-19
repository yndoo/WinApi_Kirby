#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"

// Ό³Έν :
class AFlamer : public AActor
{
public:
	// constrcuter destructer
	AFlamer();
	~AFlamer();

	// delete Function
	AFlamer(const AFlamer& _Other) = delete;
	AFlamer(AFlamer&& _Other) noexcept = delete;
	AFlamer& operator=(const AFlamer& _Other) = delete;
	AFlamer& operator=(AFlamer&& _Other) noexcept = delete;

	void Idle(float _DeltaTime);

	void IdleStart();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateUpdate(float _DeltaTime);
	void StateChange(EEnemyState _State);

	EEnemyState State = EEnemyState::None;

private:
	UImageRenderer* FlamerRenderer = nullptr;

};

