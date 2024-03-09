#pragma once
#include <EngineCore/Actor.h>
#include "MonsterHelper.h"

// 설명 :
class AMrFrosty : public MonsterHelper
{
public:
	// constrcuter destructer
	AMrFrosty();
	~AMrFrosty();

	// delete Function
	AMrFrosty(const AMrFrosty& _Other) = delete;
	AMrFrosty(AMrFrosty&& _Other) noexcept = delete;
	AMrFrosty& operator=(const AMrFrosty& _Other) = delete;
	AMrFrosty& operator=(AMrFrosty&& _Other) noexcept = delete;

	bool IsStart = false;
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void StateUpdate(float _DeltaTime) override;
	void StateChange(EEnemyState _State) override;

	void IdleStart() override;
	void Idle(float _DeltaTime) override;
	void MoveStart() override;
	void Move(float _DeltaTime) override;
	void HitWallStart();
	void HitWall(float _DeltaTime);
	void ShootReadyStart();
	void ShootReady(float _DeltaTime);
	void ShootStart();
	void Shoot(float _DeltaTime);

	void MoveUpdate(float _DeltaTime, float MaxSpeed = 0.0f, FVector Acc = FVector::Zero);
	void AddMoveVector(const FVector& _DirDelta, FVector Acc);
	void CalMoveVector(float _DeltaTime, float MaxSpeed);
	void CalGravityVector(float _DeltaTime);
	//void CalJumpVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);
	void FinalMove(float _DeltaTime);

	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 100.0f;
	FVector SmallMoveAcc = FVector::Right * 50.f;
	float MovePower = 50.f;
	float MoveMaxSpeed = 200.0f;
	FVector JumpVector = FVector::Zero;
	FVector SmallJumpPower = FVector::Up * 100;
	FVector JumpPower = FVector::Up * 400;
	//float JumpMaxSpeed = 200.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 2000.0f;
	FVector FinalMoveVector = FVector::Zero;

	UCollision* WideCollision = nullptr;	// 가까이에 커비가 있는지 확인하는 용도
	//UImageRenderer* Renderer = nullptr;
private:
	bool DirCheck() override;	// 얘는 커비를 향하는 방향
	void UpMoving(float _DeltaTime, Color8Bit _Color);
	bool IsPlayerBottomMagentaA();
	bool IsPlayerBottomYellowA();

	float Timer = 0.f;
	float ShootReadyCoolTime = 2.f;
};

