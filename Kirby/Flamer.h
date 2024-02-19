#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AFlamer : public AActor
{
public:
	// constrcuter destructer
	AFlamer();
	~AFlamer();

	// delete Function
	AFlamer(const AFlamer& _Other) = delete;
	AFlamer(AFlamer&& _Other) noexcept = delete;
	AFlamer& operator=(const AFlamer& _Other) = delete;
	AFlamer& operator=(AFlamer&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UImageRenderer* FlamerRenderer = nullptr;

};

