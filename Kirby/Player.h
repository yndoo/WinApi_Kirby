#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EngineBase/EngineTime.h>

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
	bool DirCheck();								// ���� üũ�ϰ�, ������ �ٲ�������� ����
	std::string GetAnimationName(std::string _Name);
	void RealMove(float _DeltaTime, float _MoveSpeed);				// ��¥ �̵���Ű�� �Լ�

	// ����
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);					// �÷��̾� ��/������ �̵� ����
	void Crouch(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Run(float _DeltaTime);
	void FreeMove(float _DeltaTime);	
	void CameraFreeMove(float _DeltaTime);


	// ���� ������Ʈ
	void StateUpdate(float _DeltaTime);
	void StateChange(EPlayState _State);

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

private:
	UImageRenderer* PlayerRenderer = nullptr;

	float FreeMoveSpeed = 1000.0f;

	float MoveSpeed = 300.0f;
	float SlideSpeed = 400.0f;
	float RunSpeed = 600.0f;
	float Gravity = 500.0f;

	bool IsMoveClicked = false;
	double MoveDoubleClickTime = 0;
	double SlideTime = 0;
};

