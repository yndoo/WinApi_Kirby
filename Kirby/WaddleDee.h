#pragma once
#include "MonsterHelper.h"
// Ό³Έν :
class AWaddleDee : public MonsterHelper
{
public:
	// constrcuter destructer
	AWaddleDee();
	~AWaddleDee();

	// delete Function
	AWaddleDee(const AWaddleDee& _Other) = delete;
	AWaddleDee(AWaddleDee&& _Other) noexcept = delete;
	AWaddleDee& operator=(const AWaddleDee& _Other) = delete;
	AWaddleDee& operator=(AWaddleDee&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Idle(float _DeltaTime) override;
	void Damaged(float _DeltaTime) override;
	void Move(float _DeltaTime) override;
	void Inhaled(float _DeltaTime) override;

	void IdleStart() override;
	void DamagedStart() override;
	void MoveStart() override;
	void InhaledStart() override;
private:

};

