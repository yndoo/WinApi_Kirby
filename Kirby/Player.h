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
	void GravityCheck(float _DeltaTime);
	bool DirCheck();								// 방향 체크하고, 방향이 바뀌었는지를 리턴
	std::string GetAnimationName(std::string _Name);
	void MoveUpdate(float _DeltaTime, float _MoveSpeed);				// 진짜 이동시키는 함수
	void HillMove(float _DeltaTime);

	// 상태
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);					// 플레이어 왼/오른쪽 이동 상태
	void Crouch(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Run(float _DeltaTime);
	void FreeMove(float _DeltaTime);	
	void CameraFreeMove(float _DeltaTime);


	// 상태 업데이트
	void StateUpdate(float _DeltaTime);
	void StateChange(EPlayState _State);

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

private:
	UImageRenderer* PlayerRenderer = nullptr;
	void AddMoveVector(const FVector& _DirDelta);	// 방향 벡터에 DeltaTime 곱한 값으로 들어옴
	void FinalMove(float _DeltaTime);				// 최종 계산된 방향과 힘으로 이동시키는 함수

	// 가속 운동 관련 함수들
	void CalMoveVector(float _DeltaTime);
	void CalGravityVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);

	// 가속 운동 관련 변수
	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 1000.0f;
	float MoveMaxSpeed = 300.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 1000.0f;

	FVector FinalMoveVector = FVector::Zero;
	
	
	// 보조 변수들
	float MoveSpeed = 300.0f;	// 없앨 예정?
	float SlideSpeed = 400.0f;
	float RunSpeed = 600.0f;
	float Gravity = 500.0f;
	float FreeMoveSpeed = 1000.0f;

	bool IsMoveClicked = false;
	double MoveDoubleClickTime = 0;		// 더블 클릭 인정 시간
	double SlideTime = 0;				// 슬라이딩 지속 시간

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
};

