#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EngineCore/EngineCore.h>

// 설명 :
class BulletHelper : public AActor
{
public:
	// constrcuter destructer
	BulletHelper();
	~BulletHelper();

	// delete Function
	BulletHelper(const BulletHelper& _Other) = delete;
	BulletHelper(BulletHelper&& _Other) noexcept = delete;
	BulletHelper& operator=(const BulletHelper& _Other) = delete;
	BulletHelper& operator=(BulletHelper&& _Other) noexcept = delete;

	void SetDir(EActorDir _Dir)
	{
		Dir = _Dir;
	}

protected:
	UCollision* BulletCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;
	UImageRenderer* BulletRenderer2 = nullptr;	// IceBreath에 Bullet이펙트를 두겹으로 넣어볼것임

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	virtual void StateUpdate(float _DeltaTime);
	virtual void StateChange(EBulletState _State);
	virtual bool IsWall();

	virtual void IdleStart();
	virtual void Idle(float _DeltaTime);
	virtual void MoveStart();
	virtual void Move(float _DeltaTime);
	virtual void DamageStart();
	virtual void Damage(float _DeltaTime);
	virtual void FinishStart();
	virtual void Finish(float _DeltaTime);
	virtual void InhaledStart();
	virtual void Inhaled(float _DeltaTime);

	EBulletState State = EBulletState::None;
	EActorDir Dir = EActorDir::Left;

	std::string BulletName = "";	// ex) Fire, Star
	std::string CurAnimationName = "Idle";
	std::string GetAnimationName(std::string _Name);
private:
};

