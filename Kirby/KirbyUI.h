#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"

// Ό³Έν :
class KirbyUI : public AActor
{
public:
	// constrcuter destructer
	KirbyUI();
	~KirbyUI();

	// delete Function
	KirbyUI(const KirbyUI& _Other) = delete;
	KirbyUI(KirbyUI&& _Other) noexcept = delete;
	KirbyUI& operator=(const KirbyUI& _Other) = delete;
	KirbyUI& operator=(KirbyUI&& _Other) noexcept = delete;

	void SetTypeNameUI(EKirbyType _Type);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void TypeUIUpdate();

private:
	UImageRenderer* TypeNameUI = nullptr;
	UImageRenderer* KirbyLifeUI = nullptr;
	UImageRenderer* KirbyHpUI = nullptr;

	EKirbyType TypeState = EKirbyType::None;

	FVector TypeNamePos = { -1, 500 };
};

