#pragma once
#include <EngineCore\Actor.h>

// Ό³Έν :
class AWoodBlock : public AActor
{
public:
	// constrcuter destructer
	AWoodBlock();
	~AWoodBlock();

	// delete Function
	AWoodBlock(const AWoodBlock& _Other) = delete;
	AWoodBlock(AWoodBlock&& _Other) noexcept = delete;
	AWoodBlock& operator=(const AWoodBlock& _Other) = delete;
	AWoodBlock& operator=(AWoodBlock&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UCollision* Collision = nullptr;
};

