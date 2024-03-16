#include "IceBreathBullet.h"

AIceBreathBullet::AIceBreathBullet()
{
}

AIceBreathBullet::~AIceBreathBullet()
{
}

void AIceBreathBullet::BeginPlay()
{
	BulletRenderer2 = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer2->SetImage("IceBreath_Right.png");
	BulletRenderer2->SetTransform({ {0,-30}, {300, 300} });
	BulletRenderer2->SetTransColor(Color8Bit::Magenta);

	BulletRenderer = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer->SetImage("IceBreath_Right.png");
	BulletRenderer->SetTransform({ {0,-30}, {300, 300} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);


	BulletRenderer->CreateAnimation("IceBreathIdle_Right", "IceBreath_Right.png", 0, 6, 0.05f, true);
	BulletRenderer->CreateAnimation("IceBreathIdle_Left", "IceBreath_Left.png", 0, 6, 0.05f, true);
	BulletRenderer2->CreateAnimation("IceBreathBack_Right", "IceBreath_back_Right.png", 0, 6, 0.05f, true);
	BulletRenderer2->CreateAnimation("IceBreathBack_Left", "IceBreath_back_Left.png", 0, 6, 0.05f, true);

	BulletName = "IceBreath";
	BulletRenderer->ChangeAnimation(GetAnimationName("Idle"));
	BulletRenderer2->ChangeAnimation(GetAnimationName("Back"));
	//BulletRenderer2->ActiveOff();	// 차이를 모르겠네

	BulletCollision = CreateCollision(EKirbyCollisionOrder::IceBreathBullet);
	BulletCollision->SetScale({ 60, 60 });
	BulletCollision->SetPosition({ 0, -30 });
	BulletCollision->SetColType(ECollisionType::Rect);

	StateChange(EBulletState::Idle);
}

void AIceBreathBullet::Tick(float _DeltaTime)
{
	BulletHelper::Tick(_DeltaTime);
}

void AIceBreathBullet::IdleStart()
{

}
void AIceBreathBullet::Idle(float _DeltaTime)
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
}

void AIceBreathBullet::DamageStart()
{
	
}
void AIceBreathBullet::Damage(float _DeltaTime)
{
	if (true == BulletRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}
}