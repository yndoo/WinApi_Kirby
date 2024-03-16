#pragma once
#include "BulletHelper.h"

// Ό³Έν :
class AIceBreathBullet : public BulletHelper
{
public:
	// constrcuter destructer
	AIceBreathBullet();
	~AIceBreathBullet();

	// delete Function
	AIceBreathBullet(const AIceBreathBullet& _Other) = delete;
	AIceBreathBullet(AIceBreathBullet&& _Other) noexcept = delete;
	AIceBreathBullet& operator=(const AIceBreathBullet& _Other) = delete;
	AIceBreathBullet& operator=(AIceBreathBullet&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void IdleStart() override;
	void Idle(float _DeltaTime) override;
	void DamageStart() override;
	void Damage(float _DeltaTime) override;
private:

};

