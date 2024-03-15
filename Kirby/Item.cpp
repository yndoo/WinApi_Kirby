#include "Item.h"

AItem::AItem()
{
}

AItem::~AItem()
{
}

void AItem::BeginPlay()
{

}

void AItem::Tick(float _DeltaTime)
{
	// 플레이어가 아이템 먹는 경우
	std::vector<UCollision*> Result;
	if (true == ItemCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		ItemCollision->ActiveOff();
		ItemRenderer->ActiveOff();
	}

}

void AItem::SetLifeItem()
{
	ItemRenderer = CreateImageRenderer(EKirbyRenderOrder::Item);
	ItemRenderer->SetImage("Item_Life.png");
	ItemRenderer->SetTransform({ {0,0}, {40, 40} });

	ItemCollision = CreateCollision(EKirbyCollisionOrder::LifeItem);
	ItemCollision->SetScale({ 40, 40 });
	ItemCollision->SetPosition({ 0, -40 });
	ItemCollision->SetColType(ECollisionType::Rect);
}