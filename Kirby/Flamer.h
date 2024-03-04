#pragma once
#include <EngineCore/Actor.h>
#include "ContentsHelper.h"
#include <EngineCore/EngineCore.h>
#include "MonsterHelper.h"

// 설명 :
class AFlamer : public MonsterHelper
{
public:
	// constrcuter destructer
	AFlamer();
	~AFlamer();

	// delete Function
	AFlamer(const AFlamer& _Other) = delete;
	AFlamer(AFlamer&& _Other) noexcept = delete;
	AFlamer& operator=(const AFlamer& _Other) = delete;
	AFlamer& operator=(AFlamer&& _Other) noexcept = delete;

	Color8Bit MoveColor = Color8Bit::YellowA;
	ECopyType CopyAbilityType = ECopyType::Fire;

	bool LateStart = false;
protected:


	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	//void StateUpdate(float _DeltaTime);
	//void StateChange(EEnemyState _State);

	void Idle(float _DeltaTime) override;
	void Damaged(float _DeltaTime) override;
	void Move(float _DeltaTime) override;
	void Inhaled(float _DeltaTime) override;

	void IdleStart() override;
	void DamagedStart() override;
	void MoveStart() override;
	void InhaledStart() override;

	void ColorLineMove(float _DeltaTime, Color8Bit _Color);

private:
	//FVector ActorImgScale = FVector({ 10, 10 }); // Flamer의 눈으로 보이는 크기 대충

	// 왼, 아, 오, 위
	int dx[4] = { -1, 0, 1, 0 };
	int dy[4] = { 0, 1, 0, -1 };
	int CurDir = 0; // dx[0], dy[0] : 왼
	float MoveSpeed = 100.f;

	FVector InhaleDir = FVector::Zero;
};

