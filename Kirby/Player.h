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

	bool IsPlayerBottomMagentaA();
	bool IsPlayerBottomYellow();
	bool IsPlayerDoor();
	bool IsPlayerTopMagentaA();
	void CameraMove(FVector MovePos);

	FVector BeforePos = { 100, 100 };

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	FVector MapSize; // = UContentsHelper::ColMapImage->GetScale();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// 애니메이션 LR 자동 함수들
	void AutoCreateAnimation(std::string_view _AnimationName, std::vector<int> _Indexs, float _Inter, bool _Loop);
	void AutoCreateAnimation(std::string_view _AnimationName, int _Start, int _End, float _Inter, bool _Loop);
	void AutoCreateAnimation(std::string_view _AnimationName, std::string_view _ImageName, std::vector<int> _Indexs, float _Inter, bool _Loop);
	void AutoCreateAnimation(std::string_view _AnimationName, std::string_view _ImageName, int _Start, int _End, float _Inter, bool _Loop);

	// 커비 상태 복사해주는 함수
	void KirbyCopy();

	// 상태 보조 함수
	bool DirCheck(); // 방향 체크하고, 방향이 바뀌었는지를 리턴
	std::string GetAnimationName(std::string _Name);
	void MoveUpdate(float _DeltaTime, float MaxSpeed = 0.0f, FVector Acc = FVector::Zero);	// 진짜 이동시키는 함수
	void UpMoving(float _DeltaTime, Color8Bit _Color);

	// 상태
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime); // 플레이어 왼/오른쪽 이동 상태
	void Crouch(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Run(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Break(float _DeltaTime);
	void Inhale(float _DeltaTime);
	void Eating(float _DeltaTime);
	void Swallow(float _DeltaTime);
	void Attack(float _DeltaTime);
	void Fly(float _DeltaTime);
	void LadderUp(float _DeltaTime);
	void LadderDown(float _DeltaTime);
	

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
	void InhaleStart();
	void EatingStart();
	void SwallowStart();
	void AttackStart();
	void FlyStart();
	void LadderUpStart();
	void LadderDownStart();

	// 상태 업데이트
	void StateUpdate(float _DeltaTime);
	void StateChange(EKirbyState _State);

	EKirbyState State = EKirbyState::None;
	EKirbyState BeforeJumpState = EKirbyState::None;				// 점프 이전 State가 무엇인지 저장
	EActorDir DirState = EActorDir::Right;						// DirCheck에 쓰이는 DirState
	EActorDir MyDir = EActorDir::Right;							// Move, Run 함수에서 쓸 Dir
	std::string CurAnimationName = "Idle";

	// 변신 관련 상태 변수
	bool IsEating = false;
	bool IsFireKirby = false;
private:
	UCollision* BodyCollision = nullptr;
	UCollision* BottomCollision = nullptr;
	UCollision* InhaleCollision = nullptr;
	UImageRenderer* PlayerRenderer = nullptr;

	void AddMoveVector(const FVector& _DirDelta, FVector Acc);	// 방향 벡터에 DeltaTime 곱한 값으로 들어옴
	void FinalMove(float _DeltaTime);							// 최종 계산된 방향과 힘으로 이동시키는 함수

	// 가속 운동 관련 함수들
	void CalMoveVector(float _DeltaTime, float MaxSpeed);
	void CalGravityVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);

	// 가속 운동 관련 변수
	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 200.0f;
	float MovePower = 150.f;
	float MoveMaxSpeed = 200.0f;
	FVector RunVector = FVector::Zero;
	FVector RunAcc = FVector::Right * 2000.0f;
	float RunMaxSpeed = 400.0f;
	FVector SlideVector = FVector::Zero;
	FVector SlideAcc = FVector::Right * 2000.0f;
	float SlideMaxSpeed = 800.0f;
	FVector JumpVector = FVector::Zero;
	FVector JumpPower = FVector::Up * 700.0f;	//600
	float JumpMaxSpeed = 500.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 2000.0f;
	FVector LadderUpSpeed = FVector::Up * 100.f;
	FVector LadderDownSpeed = FVector::Down * 100.f;
	FVector FinalMoveVector = FVector::Zero;
	float FreeMoveSpeed = 1000.0f;
	float FlySpeed = 100.f;
	

	float InhaleScaleVar = 40.f;
	float InhaleScaleAdd = 100.f;
	float InhaleFirstMax = 50.f;
	float InhaleSecondMax = 80.f;
	float InhaleMaxScale = 100.f;
	float FireTime = 0.0f;			// FireKirby 공격 텀에 사용되는 변수


	float LadderTop = 0.f;
	float LadderBottom = 0.f;
};

extern APlayer* Kirby;