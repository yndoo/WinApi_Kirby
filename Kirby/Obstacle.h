#pragma once
#include <EngineCore\Actor.h>

// Ό³Έν :
class AObstacle : public AActor
{
public:
	// constrcuter destructer
	AObstacle();
	~AObstacle();

	// delete Function
	AObstacle(const AObstacle& _Other) = delete;
	AObstacle(AObstacle&& _Other) noexcept = delete;
	AObstacle& operator=(const AObstacle& _Other) = delete;
	AObstacle& operator=(AObstacle&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UCollision* Collision = nullptr;

	bool Touched = false;
	float OffTime = 1.5f;
};

