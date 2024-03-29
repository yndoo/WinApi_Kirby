#pragma once
#include <EngineCore/Actor.h>

// 설명 :
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

	//UCollision* DoorCollision = nullptr;				// Door에 Collision 사용 안 하지만 혹시 모름
	UImageRenderer* StarRenderer = nullptr;				// 문 위에 별 렌더러
	UImageRenderer* EndingStarRenderer = nullptr;		// 문 위에 별 렌더러, 엔딩 포탈에만
	UImageRenderer* DoorRenderer = nullptr;				// 문 렌더러
private:

};

