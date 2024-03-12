#pragma once
#include "BulletHelper.h"

// Ό³Έν :
class AIceBullet : public BulletHelper
{
public:
	// constrcuter destructer
	AIceBullet();
	~AIceBullet();

	// delete Function
	AIceBullet(const AIceBullet& _Other) = delete;
	AIceBullet(AIceBullet&& _Other) noexcept = delete;
	AIceBullet& operator=(const AIceBullet& _Other) = delete;
	AIceBullet& operator=(AIceBullet&& _Other) noexcept = delete;

	bool GetDeathCheck()
	{
		return DeathCheck;
	}
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	//void StateUpdate(float _DeltaTime) override;
	//void StateChange(EBulletState _State) override;

	void IdleStart() override;
	void Idle(float _DeltaTime) override;
	void MoveStart() override;
	void Move(float _DeltaTime) override;
	void DamageStart() override;
	void Damage(float _DeltaTime) override;
	//void FinishStart() override;
	//void Finish(float _DeltaTime) override;

	void MoveUpdate(float _DeltaTime, float MaxSpeed = 0.0f, FVector Acc = FVector::Zero);
	void AddMoveVector(const FVector& _DirDelta, FVector Acc);
	void CalMoveVector(float _DeltaTime, float MaxSpeed);
	void CalGravityVector(float _DeltaTime);
	void CalFinalMoveVector(float _DeltaTime);
	void FinalMove(float _DeltaTime);

	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 50.0f;
	//FVector SmallMoveAcc = FVector::Right * 50.f;
	float MovePower = 50.f;
	//float MoveMaxSpeed = 200.0f;
	FVector JumpVector = FVector::Zero;
	FVector SmallJumpPower = FVector::Up * 300;
	FVector JumpPower = FVector::Up * 800;
	//float JumpMaxSpeed = 200.0f;
	FVector GravityVector = FVector::Zero;
	FVector GravityAcc = FVector::Down * 3000.0f;
	FVector FinalMoveVector = FVector::Zero;

private:
	void UpMoving(float _DeltaTime, Color8Bit _Color);
	bool IsPlayerBottomMagentaA();
	bool IsPlayerBottomYellowA();

	bool DeathCheck = false;
};

