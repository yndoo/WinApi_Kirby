#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/EngineCore.h>
#include "ContentsHelper.h"

// Ό³Έν :
class MonsterHelper : public AActor
{
public:
	// constrcuter destructer
	MonsterHelper();
	~MonsterHelper();

	// delete Function
	MonsterHelper(const MonsterHelper& _Other) = delete;
	MonsterHelper(MonsterHelper&& _Other) noexcept = delete;
	MonsterHelper& operator=(const MonsterHelper& _Other) = delete;
	MonsterHelper& operator=(MonsterHelper&& _Other) noexcept = delete;

	int GetMaxHp()
	{
		return MaxHp;
	}

	void SetMaxHp(int _MaxHp)
	{
		MaxHp = _MaxHp;
		CurHp = MaxHp;
	}

	int GetCurHp()
	{
		return CurHp;
	}

	void SetCurHp(int _CurHp)
	{
		CurHp = _CurHp;
	}

	void AddDamageHp(int _Damage)
	{
		CurHp += _Damage;
		if (CurHp > MaxHp)
		{
			CurHp = MaxHp;
		}
	}
protected:
	UCollision* MonsterCollision = nullptr;
	UImageRenderer* MonsterRenderer = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void StateUpdate(float _DeltaTime);
	void StateChange(EEnemyState _State);

	virtual void Idle(float _DeltaTime);
	virtual void Damaged(float _DeltaTime);
	virtual void Move(float _DeltaTime);
	virtual void Inhaled(float _DeltaTime);
	//virtual void Die(float _DeltaTime);
	
	virtual void IdleStart();
	virtual void DamagedStart();
	virtual void MoveStart();
	virtual void InhaledStart();
	//virtual void DieStart();

	void FallDown(Color8Bit _Color);
	std::string GetAnimationName(std::string _Name);

	EActorDir DirState = EActorDir::Left;
	EEnemyState State = EEnemyState::None;
	FVector InhaleDir = FVector::Zero;
	std::string CurAnimationName = "Idle";

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	FVector MapSize; // = UContentsHelper::ColMapImage->GetScale();
private:
	int MaxHp = 200;
	int CurHp = 200;
};

