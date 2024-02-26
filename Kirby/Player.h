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

	bool IsPlayerBottomMagentaA();
	bool IsPlayerBottomYellow();
	bool IsPlayerOnYellow();
	bool IsPlayerDoor();

	FVector BeforePos = { 100, 100 };

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	FVector MapSize; // = UContentsHelper::ColMapImage->GetScale();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void AutoCreateAnimation(std::string_view _AnimationName, std::vector<int> _Indexs, float _Inter, bool _Loop);
	void AutoCreateAnimation(std::string_view _AnimationName, int _Start, int _End, float _Inter, bool _Loop);
	void AutoCreateAnimation(std::string_view _AnimationName, std::string_view _ImageName, std::vector<int> _Indexs, float _Inter, bool _Loop);
	void AutoCreateAnimation(std::string_view _AnimationName, std::string_view _ImageName, int _Start, int _End, float _Inter, bool _Loop);

	// ���� ���� �Լ�
	bool DirCheck(); // ���� üũ�ϰ�, ������ �ٲ�������� ����
	std::string GetAnimationName(std::string _Name);
	void MoveUpdate(float _DeltaTime, float MaxSpeed = 0.0f, FVector Acc = FVector::Zero);	// ��¥ �̵���Ű�� �Լ�
	void UpMoving(float _DeltaTime, Color8Bit _Color);

	// ����
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime); // �÷��̾� ��/������ �̵� ����
	void Crouch(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Run(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Break(float _DeltaTime);
	void Inhale(float _DeltaTime);
	void Eating(float _DeltaTime);
	void EatingRun(float _DeltaTime);
	void Swallow(float _DeltaTime);

	void FreeMove(float _DeltaTime);	
	void CameraFreeMove(float _DeltaTime);

	// ���� ����
	void IdleStart();
	void MoveStart();
	void SlideStart();
	void RunStart();
	void CrouchStart();
	void JumpStart();
	void BreakStart();
	void InhaleStart();
	void EatingStart();
	void EatingRunStart();
	void SwallowStart();

	// ���� ������Ʈ
	void StateUpdate(float _DeltaTime);
	void StateChange(EPlayState _State);

	EPlayState State = EPlayState::None;
	EPlayState BeforeJumpState = EPlayState::None;				// ���� ���� State�� �������� ����
	EActorDir DirState = EActorDir::Right;						// DirCheck�� ���̴� DirState
	EActorDir MyDir = EActorDir::Right;							// Move, Run �Լ����� �� Dir
	std::string CurAnimationName = "Idle";

	bool IsEating = false;
private:
	UCollision* BodyCollision = nullptr;
	UCollision* InhaleCollision = nullptr;
	UImageRenderer* PlayerRenderer = nullptr;

	void AddMoveVector(const FVector& _DirDelta, FVector Acc);	// ���� ���Ϳ� DeltaTime ���� ������ ����
	void FinalMove(float _DeltaTime);							// ���� ���� ����� ������ �̵���Ű�� �Լ�

	// ���� � ���� �Լ���
	void CalMoveVector(float _DeltaTime, float MaxSpeed);
	void CalGravityVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);

	// ���� � ���� ����
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
	FVector JumpPower = FVector::Up * 600.0f;
	float JumpMaxSpeed = 500.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 2000.0f;
	FVector FinalMoveVector = FVector::Zero;
	float FreeMoveSpeed = 1000.0f;

	float InhaleScaleVar = 40.f;
	float InhaleScaleAdd = 100.f;
	float InhaleFirstMax = 50.f;
	float InhaleSecondMax = 80.f;
	float InhaleMaxScale = 100.f;
};

extern APlayer* Kirby;