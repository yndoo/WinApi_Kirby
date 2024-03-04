#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"

// Ό³Έν :
class ABullet : public AActor
{
public:
	// constrcuter destructer
	ABullet();
	~ABullet();

	// delete Function
	ABullet(const ABullet& _Other) = delete;
	ABullet(ABullet&& _Other) noexcept = delete;
	ABullet& operator=(const ABullet& _Other) = delete;
	ABullet& operator=(ABullet&& _Other) noexcept = delete;
	
	void SetDir(EActorDir _Dir)
	{
		Dir = _Dir;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateUpdate(float _DeltaTime);
	void StateChange(EBulletState _State);

	EBulletState State = EBulletState::None;

	void IdleStart();
	void DamageStart();
	void Idle(float _DeltaTime);
	void Damage(float _DeltaTime);
private:
	UCollision* BulletCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;

	EActorDir Dir = EActorDir::Left;
};

