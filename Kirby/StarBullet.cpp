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
	//BulletRenderer->SetImage("StarBulletIdle.png");
	BulletRenderer->SetTransform({ {0,-10}, {300, 300} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);

	BulletRenderer->CreateAnimation("StarBulletIdle_Right", "StarBulletIdle_Right.png", 0, 1, 0.2f, true);
	BulletRenderer->CreateAnimation("StarBulletIdle_Left", "StarBulletIdle_Left.png", 0, 1, 0.2f, true);

	BulletName = "StarBullet";
	BulletRenderer->ChangeAnimation(GetAnimationName("Idle"));

	BulletCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);
	BulletCollision->SetScale({ 60, 60 });
	BulletCollision->SetPosition({ 0, -10 });
	BulletCollision->SetColType(ECollisionType::Rect);

	StateChange(EBulletState::Idle);
}

void AStarBullet::Tick(float _DeltaTime)
{
	BulletHelper::Tick(_DeltaTime);
}