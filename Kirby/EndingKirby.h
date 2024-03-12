#pragma once
#include <EngineCore/Actor.h>

// ���� : �Է¾��� �������� �ϴ� Ŀ��
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

	UImageRenderer* Renderer = nullptr;	// Ŀ�� ������
	UImageRenderer* EffectRenderer1 = nullptr;
	UImageRenderer* EffectRenderer2 = nullptr;
private:

};

