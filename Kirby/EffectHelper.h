#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class EffectHelper : public AActor
{
public:
	// constrcuter destructer
	EffectHelper();
	~EffectHelper();

	// delete Function
	EffectHelper(const EffectHelper& _Other) = delete;
	EffectHelper(EffectHelper&& _Other) noexcept = delete;
	EffectHelper& operator=(const EffectHelper& _Other) = delete;
	EffectHelper& operator=(EffectHelper&& _Other) noexcept = delete;

	UImageRenderer* EffectRenderer1 = nullptr;
	UImageRenderer* EffectRenderer2 = nullptr;
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

