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

	void Idle(float _DeltaTime);
	void Hurt(float _DeltaTime);
	void Move(float _DeltaTime);
	void ColorMove(float _DeltaTime, Color8Bit _Color);

	void IdleStart();
	void HurtStart();
	void MoveStart();


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateUpdate(float _DeltaTime);
	void StateChange(EEnemyState _State);

	EEnemyState State = EEnemyState::None;

private:
	UCollision* FlamerCollision;
	UImageRenderer* FlamerRenderer = nullptr;
	//FVector ActorImgScale = FVector({ 10, 10 }); // Flmer의 눈으로 보이는 크기 대충

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	FVector MapSize; // = UContentsHelper::ColMapImage->GetScale();

	// 왼, 아, 오, 위
	int dx[4] = { -1, 0, 1, 0 };
	int dy[4] = { 0, 1, 0, -1 };
	int CurDir = 0; // dx[0], dy[0] : 왼
	float MoveSpeed = 0.17f;

	FVector InhaleDir = FVector::Zero;
};

