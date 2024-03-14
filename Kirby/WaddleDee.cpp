#include "WaddleDee.h"

AWaddleDee::AWaddleDee()
{
}

AWaddleDee::~AWaddleDee()
{
}

void AWaddleDee::BeginPlay()
{
	AActor::BeginPlay();

	SetActorLocation({ 1500, 200 });
	SetMaxHp(50);

	MonsterRenderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	MonsterRenderer->SetImage("WaddleDee_Right.png");
	MonsterRenderer->SetTransform({ {0,0}, {300, 300} });
	MonsterRenderer->SetTransColor(Color8Bit::Magenta);

	MonsterRenderer->CreateAnimation("Idle_Right", "WaddleDee_Right.png", 0, 4, 0.3f, true);
	MonsterRenderer->CreateAnimation("Idle_Left", "WaddleDee_Left.png", 0, 4, 0.3f, true);
	MonsterRenderer->CreateAnimation("Inhaled_Right", "WaddleDee_Right.png", 8, 8, 0.1f, false);
	MonsterRenderer->CreateAnimation("Inhaled_Left", "WaddleDee_Left.png", 8, 8, 0.1f, false);
	MonsterRenderer->CreateAnimation("Damaged_Right", "WaddleDee_Right.png", { 8,8,8,8 }, 0.1f, false);
	MonsterRenderer->CreateAnimation("Damaged_Left", "WaddleDee_Left.png", { 8,8,8,8 }, 0.1f, false);
	MonsterRenderer->CreateAnimation("DieEffect", "MonDieEffects.png", 0, 23, 0.05f, false);
	
	MonsterCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	MonsterCollision->SetScale({ 40, 40 });
	MonsterCollision->SetPosition({ 0, -20 });
	MonsterCollision->SetColType(ECollisionType::Rect);

	MapSize = UContentsHelper::ColMapImage->GetScale();

	FallDown(Color8Bit::MagentaA);
	StateChange(EEnemyState::Idle);
}
void AWaddleDee::Tick(float _DeltaTime)
{
	MonsterHelper::Tick(_DeltaTime);
}

void AWaddleDee::IdleStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
}
void AWaddleDee::Idle(float _DeltaTime)
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));

	FVector ActorPos = GetActorLocation();
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(ActorPos.iX(), ActorPos.iY(), Color8Bit::MagentaA);
	if (Color != Color8Bit::MagentaA)
	{
		FallDown(Color8Bit::MagentaA);
	}

	// 벽에 부딪히면 반대로 이동하도록.
	if (true == IsWall())
	{
		switch (MonsterDir)
		{
		case EActorDir::Left:
			MonsterDir = EActorDir::Right;
			break;
		case EActorDir::Right:
			MonsterDir = EActorDir::Left;
			break;
		default:
			break;
		}
	}

	switch (MonsterDir)
	{
	case EActorDir::Left:
		AddActorLocation(FVector::Left * 50.0f * _DeltaTime);
		break;
	case EActorDir::Right:
		AddActorLocation(FVector::Right * 50.0f * _DeltaTime);
		break;
	default:
		break;
	}
}

void AWaddleDee::MoveStart()
{
}
void AWaddleDee::Move(float _DeltaTime)
{

}

void AWaddleDee::InhaledStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Inhaled"));
}
void AWaddleDee::Inhaled(float _DeltaTime)
{
	// 흡입되다가 커비 몸과 충돌됐을 경우
	std::vector<UCollision*> Result;
	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		//StateChange(EEnemyState::Damaged);
		MonsterRenderer->ActiveOff();
		MonsterCollision->ActiveOff();
		return;
	}
}

void AWaddleDee::DamagedStart()
{
	AddDamageHp(60);
	MonsterRenderer->ChangeAnimation(GetAnimationName("Damaged"));
	IsDamaged = true;
}
void AWaddleDee::Damaged(float _DeltaTime)
{
	if (GetCurHp() <= 0 && true == MonsterRenderer->IsCurAnimationEnd())
	{
		// 원래 게임 : 한 대 맞고 이펙트터지면서 죽어야 함. 
		StateChange(EEnemyState::Die);
	}
}

void AWaddleDee::DieStart()
{
	MonsterRenderer->ChangeAnimation("DieEffect");
	MonsterRenderer->AddPosition({ 0, -20 });
	DeathCheck = true;
}
void AWaddleDee::Die(float _DeltaTime)
{
	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		MonsterCollision->ActiveOff();
		MonsterRenderer->ActiveOff();
		//Destroy();
		//StateChange(EEnemyState::None);
		return;
	}
}

bool AWaddleDee::IsWall()
{
	FVector CurPos = GetActorLocation();
	Color8Bit RColor = UContentsHelper::ColMapImage->GetColor(CurPos.iX() + 30, CurPos.iY() - 10, Color8Bit::MagentaA);
	Color8Bit LColor = UContentsHelper::ColMapImage->GetColor(CurPos.iX() - 30, CurPos.iY() - 10, Color8Bit::MagentaA);

	switch (MonsterDir)
	{
	case EActorDir::Left:
		if (LColor == Color8Bit::MagentaA)
		{
			return true;
		}
		break;
	case EActorDir::Right:
		if (RColor == Color8Bit::MagentaA)
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}