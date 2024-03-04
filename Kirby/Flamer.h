#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EngineCore/EngineCore.h>

// 설명 :
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

	Color8Bit MoveColor = Color8Bit::YellowA;
	ECopyType CopyAbilityType = ECopyType::Fire;

	bool LateStart = false;
protected:
	UCollision* MonsterCollision = nullptr;
	UImageRenderer* MonsterRenderer = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void StateUpdate(float _DeltaTime);
	void StateChange(EEnemyState _State);

	void Idle(float _DeltaTime);
	void Damaged(float _DeltaTime);
	void Move(float _DeltaTime);
	void Inhaled(float _DeltaTime);

	void IdleStart();
	void DamagedStart();
	void MoveStart();
	void InhaledStart();

	void FallDown(Color8Bit _Color);
	void ColorLineMove(float _DeltaTime, Color8Bit _Color);

	EEnemyState State = EEnemyState::None;

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	FVector MapSize; // = UContentsHelper::ColMapImage->GetScale();
private:
	//FVector ActorImgScale = FVector({ 10, 10 }); // Flamer의 눈으로 보이는 크기 대충

	// 왼, 아, 오, 위
	int dx[4] = { -1, 0, 1, 0 };
	int dy[4] = { 0, 1, 0, -1 };
	int CurDir = 0; // dx[0], dy[0] : 왼
	float MoveSpeed = 100.f;

	FVector InhaleDir = FVector::Zero;
};

