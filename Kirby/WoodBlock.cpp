#include "WoodBlock.h"
#include "ContentsHelper.h"

AWoodBlock::AWoodBlock()
{
}

AWoodBlock::~AWoodBlock()
{
}

void AWoodBlock::BeginPlay()
{
	AActor::BeginPlay();

	Collision = CreateCollision(EKirbyCollisionOrder::Block);
	Collision->SetScale({ 40, 4 });
	Collision->SetPosition({ 20, 2 });	// 액터위치를 좌상단으로 잡기 때문에 Scale의 반씩 더해줌.
	Collision->SetColType(ECollisionType::Rect);
}

void AWoodBlock::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}