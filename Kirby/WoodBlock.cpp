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
	Collision->SetScale({ 40, 200 });
	Collision->SetPosition({ 0, 0 });
	Collision->SetColType(ECollisionType::Rect);
}

void AWoodBlock::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}