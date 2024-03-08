#pragma once
#include <EngineCore/Actor.h>
#include "MonsterHelper.h"

// Ό³Έν :
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

	void Idle(float _DeltaTime) override;
	void IdleStart() override;
	void Move(float _DeltaTime) override;
	void MoveStart() override;

	void MoveUpdate(float _DeltaTime, float MaxSpeed = 0.0f, FVector Acc = FVector::Zero);
	void AddMoveVector(const FVector& _DirDelta, FVector Acc);
	void CalMoveVector(float _DeltaTime, float MaxSpeed);
	void CalGravityVector(float _DeltaTime);
	void CalJumpVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);
	void FinalMove(float _DeltaTime);

	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 100.0f;
	float MovePower = 50.f;
	float MoveMaxSpeed = 200.0f;
	FVector JumpVector = FVector::Zero;
	FVector SmallJumpPower = FVector::Up * 100;
	FVector JumpPower = FVector::Up * 400;
	float JumpMaxSpeed = 200.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 2000.0f;
	FVector FinalMoveVector = FVector::Zero;

	//UCollision* BodyCollision = nullptr;
	//UImageRenderer* Renderer = nullptr;
private:
	void UpMoving(float _DeltaTime, Color8Bit _Color);
	bool IsPlayerBottomMagentaA();
	bool IsPlayerBottomYellowA();
};

