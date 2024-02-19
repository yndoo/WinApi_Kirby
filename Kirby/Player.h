#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EngineBase/EngineTime.h>
#include <EngineCore/EngineCore.h>

// 설명 :
class APlayer : public AActor
{
public:
	// constrcuter destructer
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// 상태 보조 함수
	bool DirCheck();								// 방향 체크하고, 방향이 바뀌었는지를 리턴
	std::string GetAnimationName(std::string _Name);
	void MoveUpdate(float _DeltaTime, float MaxSpeed = 0.0f, FVector Acc = FVector::Zero);				// 진짜 이동시키는 함수
	void HillMove(float _DeltaTime);

	// 상태
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);					// 플레이어 왼/오른쪽 이동 상태
	void Crouch(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Run(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Break(float _DeltaTime);
	void FreeMove(float _DeltaTime);	
	void CameraFreeMove(float _DeltaTime);

	// 상태 시작
	void IdleStart();
	void MoveStart();
	void SlideStart();
	void RunStart();
	void CrouchStart();
	void JumpStart();
	void BreakStart();

	// 상태 업데이트
	void StateUpdate(float _DeltaTime);
	void StateChange(EPlayState _State);

	EPlayState State = EPlayState::None;
	EPlayState BeforeJumpState = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

private:
	UImageRenderer* PlayerRenderer = nullptr;
	void AddMoveVector(const FVector& _DirDelta, FVector Acc);	// 방향 벡터에 DeltaTime 곱한 값으로 들어옴
	void FinalMove(float _DeltaTime);				// 최종 계산된 방향과 힘으로 이동시키는 함수

	// 가속 운동 관련 함수들
	void CalMoveVector(float _DeltaTime, float MaxSpeed);
	void CalGravityVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);

	// 가속 운동 관련 변수
	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 200.0f;
	float MoveMaxSpeed = 200.0f;
	FVector RunVector = FVector::Zero;
	FVector RunAcc = FVector::Right * 2000.0f;
	float RunMaxSpeed = 400.0f;
	FVector SlideVector = FVector::Zero;
	FVector SlideAcc = FVector::Right * 2000.0f;
	float SlideMaxSpeed = 800.0f;
	FVector JumpVector = FVector::Zero;
	FVector JumpPower = FVector::Up * 500.0f;
	float JumpMaxSpeed = 600.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 1000.0f;

	FVector FinalMoveVector = FVector::Zero;
	
	
	// 보조 변수들
	float Gravity = 500.0f;
	float FreeMoveSpeed = 1000.0f;

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	FVector MapSize = UContentsHelper::ColMapImage->GetScale();
};

