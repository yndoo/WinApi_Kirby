#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"

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
	void GravityCheck(float _DeltaTime);
	void DirCheck();
	std::string GetAnimationName(std::string _Name);

	// ����
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);

	// ���� ������Ʈ
	void StateUpdate(float _DeltaTime);
	void StateChange(EPlayState _State);

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

private:
	UImageRenderer* PlayerRenderer = nullptr;

	float MoveSpeed = 300.0f;
	float Gravity = 500.0f;
};

