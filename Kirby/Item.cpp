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
	if (ItemCollision->GetOrder() == static_cast<int>(EKirbyCollisionOrder::IceTypeItem) || ItemCollision->GetOrder() == static_cast<int>(EKirbyCollisionOrder::FireTypeItem))
	{
		Angle = _DeltaTime * 80.f;
		SpinDir.RotationZToDeg(Angle);
		SetActorLocation(Center + SpinDir);
	}

	// 플레이어가 아이템 먹는 경우
	std::vector<UCollision*> Result;
	if (true == ItemCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		ItemRenderer->ActiveOff();
		ItemCollision->ActiveOff();
	}
}

void AItem::SetLifeItem()
{
	ItemRenderer = CreateImageRenderer(EKirbyRenderOrder::Item);
	ItemRenderer->SetImage("Item_Life.png");
	ItemRenderer->SetTransform({ {0,0}, {40, 40} });

	ItemCollision = CreateCollision(EKirbyCollisionOrder::LifeItem);
	ItemCollision->SetScale({ 40, 40 });
	ItemCollision->SetPosition({ 0, 0 });
	ItemCollision->SetColType(ECollisionType::Rect);
}

void AItem::SetIceTypeItem()
{
	ItemRenderer = CreateImageRenderer(EKirbyRenderOrder::Item);
	ItemRenderer->SetImage("Item_IceCopy.png");
	ItemRenderer->CreateAnimation("ItemAni", "Item_IceCopy_ani.png", {0,1,2,3,0,1,2,3,3,3,3,3,3,4,3,4,3}, 0.06f, true);
	ItemRenderer->ChangeAnimation("ItemAni");
	ItemRenderer->SetTransform({ {0,0}, {160, 158} });

	ItemCollision = CreateCollision(EKirbyCollisionOrder::IceTypeItem);
	ItemCollision->SetScale({ 80,0 });
	ItemCollision->SetPosition({ 0, 0 });
	ItemCollision->SetColType(ECollisionType::CirCle);

	SpinDir = (GetActorLocation() - FVector::Down).Normalize2DReturn() * 10;
}

void AItem::SetFireTypeItem()
{
	ItemRenderer = CreateImageRenderer(EKirbyRenderOrder::Item);
	ItemRenderer->SetImage("Item_FireCopy.png");
	ItemRenderer->CreateAnimation("ItemAni", "Item_FireCopy_ani.png", { 0,1,2,3,0,1,2,3,3,3,3,3,3,4,3,4,3 }, 0.06f, true);
	ItemRenderer->ChangeAnimation("ItemAni");
	ItemRenderer->SetTransform({ {0,0}, {160, 158} });

	ItemCollision = CreateCollision(EKirbyCollisionOrder::FireTypeItem);
	ItemCollision->SetScale({ 80,0 });
	ItemCollision->SetPosition({ 0, 0 });
	ItemCollision->SetColType(ECollisionType::CirCle);

	SpinDir = (GetActorLocation() - FVector::Down).Normalize2DReturn() * 10;
}