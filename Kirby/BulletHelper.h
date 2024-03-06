#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"

// ���� :
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

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateUpdate(float _DeltaTime);
	void StateChange(EBulletState _State);

	virtual void IdleStart();
	virtual void DamageStart();
	virtual void FinishStart();
	virtual void Idle(float _DeltaTime);
	virtual void Damage(float _DeltaTime);
	virtual void Finish(float _DeltaTime);

	EBulletState State = EBulletState::None;
	EActorDir Dir = EActorDir::Left;

	std::string BulletName = "";	// ex) Fire, Star
	std::string CurAnimationName = "Idle";
	std::string GetAnimationName(std::string _Name);
private:
};
