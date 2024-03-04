#include "FireBullet.h"

AFireBullet::AFireBullet()
{
}

AFireBullet::~AFireBullet()
{
}

void AFireBullet::BeginPlay()
{
	BulletRenderer = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer->SetImage("FireBullet.png");
	BulletRenderer->SetTransform({ {0,0}, {400, 400} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);

	BulletRenderer->CreateAnimation("FireBulletIdle_Right", "FireBulletIdle_Right.png", 0, 6, 0.05f, true);
	BulletRenderer->CreateAnimation("FireBulletIdle_Left", "FireBulletIdle_Left.png", 0, 6, 0.05f, true);

	BulletCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);
	BulletCollision->SetScale({ 100, 60 });
	BulletCollision->SetPosition({ 0, 0 });
	BulletCollision->SetColType(ECollisionType::Rect);

	BulletName = "FireBullet";
	//BulletRenderer->ChangeAnimation(GetAnimationName("Idle"));

	StateChange(EBulletState::Idle);
}

void AFireBullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void AFireBullet::IdleStart()
{
	
}

void AFireBullet::Idle(float _DeltaTime)
{
	if (BulletRenderer->GetCurAnimation() != nullptr && BulletRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}
	BulletRenderer->ChangeAnimation(GetAnimationName("Idle"));
	switch (Dir)
	{
	case EActorDir::Left:
		AddActorLocation(FVector::Left * 200.0f * _DeltaTime);
		break;
	case EActorDir::Right:
		AddActorLocation(FVector::Right * 200.0f * _DeltaTime);
		break;
	default:
		break;
	}

	// 공격이 Monster에 닿으면
	//std::vector<UCollision*> Result;
	//if (true == BulletCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result))
	//{
	//	AActor* temp = Result[0]->GetOwner();
	//	int a = 0;

	//	StateChange(EBulletState::Damage);
	//	return;
	//}
}