#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"

// ���� :
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
	void SetKirbyHpUI(int _CurHp);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void TypeUIUpdate();

private:
	UImageRenderer* TypeNameUI = nullptr;
	UImageRenderer* KirbyHpCaseUI = nullptr;
	UImageRenderer* KirbyHpUI = nullptr;
	UImageRenderer* KirbyLifeImgUI = nullptr;	// ��� �̹���, x �̹���
	UImageRenderer* KirbyLifeNumUI[2] = { nullptr, };	// ��� ���� ���� UI

	EKirbyType TypeState = EKirbyType::None;

	FVector TypeNamePos = { -1, 490 };
	FVector HpCasePos = { 120, 460 };
	FVector LifeImgPos = HpCasePos + FVector({ 30, -32 });
	FVector LifeNumUIPos = LifeImgPos + FVector({ 60, 6 });	
};