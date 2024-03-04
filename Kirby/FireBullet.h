#pragma once
#include "BulletHelper.h"

// Ό³Έν :
class AFireBullet : public BulletHelper
{
public:
	// constrcuter destructer
	AFireBullet();
	~AFireBullet();

	// delete Function
	AFireBullet(const AFireBullet& _Other) = delete;
	AFireBullet(AFireBullet&& _Other) noexcept = delete;
	AFireBullet& operator=(const AFireBullet& _Other) = delete;
	AFireBullet& operator=(AFireBullet&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void IdleStart() override;
	void Idle(float _DeltaTime) override;
private:

};

