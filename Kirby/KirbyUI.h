#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EngineCore/EngineCore.h>

// 설명 :
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
	void SetKirbyLifeUI(int _CurLife);
	void SetBossHpUI(int _MaxHp, int _CurHp);
	void BossUIOn();
	void MonsterAniOn();

	UImageRenderer* ManualRenderer = nullptr;
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void TypeUIUpdate();

private:
	UImageRenderer* TypeNameUI = nullptr;
	UImageRenderer* KirbyHpCaseUI = nullptr;
	UImageRenderer* KirbyHpUI = nullptr;
	UImageRenderer* KirbyLifeImgUI = nullptr;	// 목숨 이미지, x 이미지
	UImageRenderer* KirbyLifeNumUI[2] = { nullptr, };	// 목숨 개수 숫자 UI

	UImageRenderer* BossHpCaseUI = nullptr;
	UImageRenderer* BossHpBarUI = nullptr;
	UImageRenderer* MonHpRenderer = nullptr;


	EKirbyType TypeState = EKirbyType::Normal;

	FVector TypeNamePos = { -1, 490 };
	FVector HpCasePos = { 120, 460 };
	FVector LifeImgPos = HpCasePos + FVector({ 30, -32 });
	FVector LifeNumUIDefaultPos = LifeImgPos + FVector({ 60, 6 });
	FVector LifeNumUIPos = LifeImgPos + FVector({ 60, 6 });	

	FVector WinScale = GEngine->MainWindow.GetWindowScale();

	bool IsMonHpRenderOn = false;
};