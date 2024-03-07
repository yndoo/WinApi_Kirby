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
	Collision->SetPosition({ 20, 2 });	// ������ġ�� �»������ ��� ������ Scale�� �ݾ� ������.
	Collision->SetColType(ECollisionType::Rect);
}

void AWoodBlock::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}