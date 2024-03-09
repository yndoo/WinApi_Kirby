#include "IceBullet.h"

AIceBullet::AIceBullet()
{
}

AIceBullet::~AIceBullet()
{
}

void AIceBullet::BeginPlay()
{
	BulletRenderer = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer->SetImage("IceBlock.png");
	BulletRenderer->SetTransform({ {0,-30}, {60, 60} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);

	BulletCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);
	BulletCollision->SetScale({ 60, 60 });
	BulletCollision->SetPosition({ 0, -30 });
	BulletCollision->SetColType(ECollisionType::Rect);
}

void AIceBullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	switch (Dir)
	{
	case EActorDir::Left:
		AddActorLocation(FVector::Left * 100.f * _DeltaTime);
		break;
	case EActorDir::Right:
		AddActorLocation(FVector::Right * 100.f * _DeltaTime);
		break;
	default:
		break;
	}
}