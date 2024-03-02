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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UCollision* BulletCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;

};

