#pragma once
#include <EngineCore\Actor.h>

// Ό³Έν :
class ALadder : public AActor
{
public:
	// constrcuter destructer
	ALadder();
	~ALadder();

	// delete Function
	ALadder(const ALadder& _Other) = delete;
	ALadder(ALadder&& _Other) noexcept = delete;
	ALadder& operator=(const ALadder& _Other) = delete;
	ALadder& operator=(ALadder&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UCollision* Collision = nullptr;
private:

};

