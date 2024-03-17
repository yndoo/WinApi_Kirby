#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EnginePlatform\EngineSound.h>

// Ό³Έν :
class AItem : public AActor
{
public:
	// constrcuter destructer
	AItem();
	~AItem();

	// delete Function
	AItem(const AItem& _Other) = delete;
	AItem(AItem&& _Other) noexcept = delete;
	AItem& operator=(const AItem& _Other) = delete;
	AItem& operator=(AItem&& _Other) noexcept = delete;

	void SetLifeItem();
	void SetIceTypeItem();
	void SetFireTypeItem();

	void SetSpinCenter(FVector _Center);
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* ItemRenderer = nullptr;
	UCollision* ItemCollision = nullptr;
	UEngineSoundPlayer ItemSound;
private:
	float Angle = 0.f;
	FVector Center = { 184, 513 };
	FVector SpinDir = FVector::Zero;
};

