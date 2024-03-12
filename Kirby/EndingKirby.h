#pragma once
#include <EngineCore/Actor.h>

// 설명 : 입력없이 랜더링만 하는 커비
class AEndingKirby : public AActor
{
public:
	// constrcuter destructer
	AEndingKirby();
	~AEndingKirby();

	// delete Function
	AEndingKirby(const AEndingKirby& _Other) = delete;
	AEndingKirby(AEndingKirby&& _Other) noexcept = delete;
	AEndingKirby& operator=(const AEndingKirby& _Other) = delete;
	AEndingKirby& operator=(AEndingKirby&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* Renderer = nullptr;	// 커비 렌더러
	UImageRenderer* EffectRenderer1 = nullptr;
	UImageRenderer* EffectRenderer2 = nullptr;
private:

};

