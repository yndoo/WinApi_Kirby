#include "Bullet.h"

ABullet::ABullet()
{
}

ABullet::~ABullet()
{
}

void ABullet::BeginPlay()
{
	BulletRenderer = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer->SetImage("Bullet.png");
	BulletRenderer->SetTransform({ {0,0}, {400, 400} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);

	BulletRenderer->CreateAnimation("Bullet", "Bullet.png", 0, 1, 0.2f, true);
	BulletRenderer->ChangeAnimation("Bullet");

	BulletCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);
	BulletCollision->SetScale({ 100, 100 });	
	BulletCollision->SetPosition({ 0, -20 });
	BulletCollision->SetColType(ECollisionType::Rect);
	//BulletCollision->ActiveOn();
}

void ABullet::Tick(float _DeltaTime)
{

}

void  ABullet::StateChange(EBulletState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EBulletState::Idle:
			IdleStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void  ABullet::StateUpdate(float _DeltaTime)
{
	switch (State) 
	{
	case EBulletState::Idle:
		Idle(_DeltaTime);
		break;
	default:
		break;
	}
}

void ABullet::IdleStart()
{
	// 한 번 쏜 방향으로 가면 되므로 한 번 받아온 Dir로 충분함. (DirCheck 필요 없음)
	BulletRenderer->ChangeAnimation("Bullet");
}

void ABullet::DamageStart()
{

}

void ABullet::Idle(float _DeltaTime)
{
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
	std::vector<UCollision*> Result;
	if (true == BulletCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result))
	{
		AActor* temp = Result[0]->GetOwner();
		int a = 0;

		StateChange(EBulletState::Damage);
		return;
	}
}

void ABullet::Damage(float _DeltaTime)
{

}