#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"

// ���� :
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
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* ItemRenderer = nullptr;
	UCollision* ItemCollision = nullptr;
private:

};

