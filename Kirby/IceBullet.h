#pragma once
#include "BulletHelper.h"

// Ό³Έν :
class AIceBullet : public BulletHelper
{
public:
	// constrcuter destructer
	AIceBullet();
	~AIceBullet();

	// delete Function
	AIceBullet(const AIceBullet& _Other) = delete;
	AIceBullet(AIceBullet&& _Other) noexcept = delete;
	AIceBullet& operator=(const AIceBullet& _Other) = delete;
	AIceBullet& operator=(AIceBullet&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

