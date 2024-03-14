#pragma once
#include <EngineCore/Actor.h>

// ���� :
class ADoorStar : public AActor
{
public:
	// constrcuter destructer
	ADoorStar();
	~ADoorStar();

	// delete Function
	ADoorStar(const ADoorStar& _Other) = delete;
	ADoorStar(ADoorStar&& _Other) noexcept = delete;
	ADoorStar& operator=(const ADoorStar& _Other) = delete;
	ADoorStar& operator=(ADoorStar&& _Other) noexcept = delete;

	void BigStarOn();
	void SmallStarOn();
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	//UCollision* DoorCollision = nullptr;				// Door�� Collision ��� �� ������ Ȥ�� ��
	UImageRenderer* StarRenderer = nullptr;				// �� ���� �� ������
	UImageRenderer* EndingStarRenderer = nullptr;		// �� ���� �� ������, ���� ��Ż����
	UImageRenderer* DoorRenderer = nullptr;				// �� ������
private:

};

