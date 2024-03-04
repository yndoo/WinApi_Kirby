#include "StarBullet.h"

AStarBullet::AStarBullet()
{
}

AStarBullet::~AStarBullet()
{
}

void AStarBullet::BeginPlay()
{
	BulletRenderer = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer->SetImage("StarIdle.png");
	BulletRenderer->SetTransform({ {0,0}, {400, 400} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);

	BulletRenderer->CreateAnimation("StarIdle", "StarIdle.png", 0, 1, 0.2f, true);
	BulletRenderer->ChangeAnimation("StarIdle");

	BulletCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);
	BulletCollision->SetScale({ 100, 100 });
	BulletCollision->SetPosition({ 0, -20 });
	BulletCollision->SetColType(ECollisionType::Rect);
	//BulletCollision->ActiveOn();

	BulletName = "Star";
	StateChange(EBulletState::Idle);
}

void AStarBullet::Tick(float _DeltaTime)
{
	BulletHelper::Tick(_DeltaTime);
}