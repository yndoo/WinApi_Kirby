#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EngineBase/EngineTime.h>
#include <EngineCore/EngineCore.h>

// ���� :
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

	// ���� ���� �Լ�
	bool DirCheck();								// ���� üũ�ϰ�, ������ �ٲ�������� ����
	std::string GetAnimationName(std::string _Name);
	void MoveUpdate(float _DeltaTime, float MaxSpeed = 0.0f, FVector Acc = FVector::Zero);				// ��¥ �̵���Ű�� �Լ�
	void HillMove(float _DeltaTime);

	// ����
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);					// �÷��̾� ��/������ �̵� ����
	void Crouch(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Run(float _DeltaTime);
	void FreeMove(float _DeltaTime);	
	void CameraFreeMove(float _DeltaTime);

	// ���� ����
	void IdleStart();
	void MoveStart();
	void SlideStart();
	void RunStart();
	void CrouchStart();

	// ���� ������Ʈ
	void StateUpdate(float _DeltaTime);
	void StateChange(EPlayState _State);

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

private:
	UImageRenderer* PlayerRenderer = nullptr;
	void AddMoveVector(const FVector& _DirDelta, FVector Acc);	// ���� ���Ϳ� DeltaTime ���� ������ ����
	void FinalMove(float _DeltaTime);				// ���� ���� ����� ������ �̵���Ű�� �Լ�

	// ���� � ���� �Լ���
	void CalMoveVector(float _DeltaTime, float MaxSpeed, FVector Acc);
	void CalSlideVector(float _DeltaTime, float MaxSpeed, FVector Acc);
	void CalGravityVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);

	// ���� � ���� ����
	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 1000.0f;
	float MoveMaxSpeed = 300.0f;
	FVector RunVector = FVector::Zero;
	FVector RunAcc = FVector::Right * 2000.0f;
	float RunMaxSpeed = 600.0f;
	FVector SlideVector = FVector::Zero;
	FVector SlideAcc = FVector::Right * 2000.0f;
	float SlideMaxSpeed = 800.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 1000.0f;

	FVector FinalMoveVector = FVector::Zero;
	
	
	// ���� ������
	float SlideSpeed = 400.0f;
	float Gravity = 500.0f;
	float FreeMoveSpeed = 1000.0f;

	bool IsMoveClicked = false;
	double MoveDoubleClickTime = 0;		// ���� Ŭ�� ���� �ð�
	double SlideTime = 0;				// �����̵� ���� �ð�

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
};

