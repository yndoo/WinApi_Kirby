#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ASpawner : public AActor
{
public:
	// constrcuter destructer
	ASpawner();
	~ASpawner();

	// delete Function
	ASpawner(const ASpawner& _Other) = delete;
	ASpawner(ASpawner&& _Other) noexcept = delete;
	ASpawner& operator=(const ASpawner& _Other) = delete;
	ASpawner& operator=(ASpawner&& _Other) noexcept = delete;

	UCollision* SpawnCollision = nullptr;
protected:

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

