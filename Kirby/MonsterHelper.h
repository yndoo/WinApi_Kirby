#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/EngineCore.h>
#include "ContentsHelper.h"

// 설명 :
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
		if (CurHp > MaxHp)
		{
			CurHp = MaxHp;
		}
		else if (CurHp < 0)
		{
			CurHp = 0;
		}
	}

	// 체력 깎는 == 데미지 입히는 함수
	void AddDamageHp(int _Damage)
	{
		SetCurHp(CurHp - _Damage);
	}

	bool DeathCheck = false;
	bool IsDamaged = false;
protected:
	UCollision* MonsterCollision = nullptr;
	UCollision* MonsterFrontCollision = nullptr;	// 콜리전 잔상(동시충돌 버그때문)
	UCollision* IceCollision = nullptr;				// 얼려졌을 경우
	UCollision* IcePlayerBulletCollision = nullptr; // 얼려져서 공격체가 된 경우
	UImageRenderer* MonsterRenderer = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	virtual void StateUpdate(float _DeltaTime);
	virtual void StateChange(EEnemyState _State);
	virtual bool IsWall();

	virtual void Idle(float _DeltaTime);
	virtual void Damaged(float _DeltaTime);
	virtual void Move(float _DeltaTime);
	virtual void Inhaled(float _DeltaTime);
	virtual void Die(float _DeltaTime);
	virtual void Iced(float _DeltaTime);
	
	virtual void IdleStart();
	virtual void DamagedStart();
	virtual void MoveStart();
	virtual void InhaledStart();
	virtual void DieStart();
	virtual void IcedStart();

	virtual bool DirCheck();	// 커비의 방향 알려주는 함수
	virtual std::string GetAnimationName(std::string _Name);
	void FallDown(Color8Bit _Color);
	void SwitchIsDamaged(float _DeltaTime, float _CoolTime);

	EActorDir DirState = EActorDir::Left;	// 커비의 Dir
	EActorDir MonsterDir = EActorDir::Left;	// 몬스터의 현재 방향
	EEnemyState State = EEnemyState::None;
	FVector InhaleDir = FVector::Zero;
	std::string CurAnimationName = "Idle";
	float SwitchIsDamagedTimer = 0.f;
	float InhaleSpeed = 200.f;

	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	FVector MapSize; // = UContentsHelper::ColMapImage->GetScale();
private:
	int MaxHp = 200;
	int CurHp = 200;

	bool IsIced = false;
	bool IcePushed = false;
};

