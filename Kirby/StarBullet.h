#pragma once
#include "BulletHelper.h"

// Ό³Έν :
class AStarBullet : public BulletHelper
{
public:
	// constrcuter destructer
	AStarBullet();
	~AStarBullet();

	// delete Function
	AStarBullet(const AStarBullet& _Other) = delete;
	AStarBullet(AStarBullet&& _Other) noexcept = delete;
	AStarBullet& operator=(const AStarBullet& _Other) = delete;
	AStarBullet& operator=(AStarBullet&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void DamageStart() override;
	void Damage(float _DeltaTime) override;
private:

};

