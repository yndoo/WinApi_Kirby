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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void StateUpdate(float _DeltaTime) override;
	void StateChange(EEnemyState _State) override;

	void Idle(float _DeltaTime) override;
	void IdleStart() override;

	UCollision* BodyCollision = nullptr;
	UImageRenderer* Renderer = nullptr;
private:

};

