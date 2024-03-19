#include "WaddleDee.h"

AWaddleDee::AWaddleDee()
{
}

AWaddleDee::~AWaddleDee()
{
}

void AWaddleDee::BeginPlay()
{
	MonsterHelper::BeginPlay();

	//SetActorLocation({ 1500, 200 });
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

	MonsterFrontCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	MonsterFrontCollision->SetScale({ 40, 40 });
	MonsterFrontCollision->SetPosition({ 0, -20 });
	MonsterFrontCollision->SetColType(ECollisionType::Rect);
	MonsterFrontCollision->ActiveOff();

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

	// ���� �ε����� �ݴ�� �̵��ϵ���.
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
	// ���ԵǴٰ� Ŀ�� ���� �浹���� ���
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
	IsDamaged = true;
	AddDamageHp(60);
	MonsterFrontCollision->ActiveOn();
	MonsterCollision->ActiveOff();
	MonsterRenderer->ChangeAnimation(GetAnimationName("Damaged"));
}
void AWaddleDee::Damaged(float _DeltaTime)
{
	if (GetCurHp() <= 0 && true == MonsterRenderer->IsCurAnimationEnd())
	{
		StateChange(EEnemyState::Die);
	}
}

void AWaddleDee::DieStart()
{
	MonsterFrontCollision->ActiveOff();
	if (IceCollision != nullptr)
	{
		IceCollision->ActiveOff();
	}
	if (IcePlayerBulletCollision != nullptr)
	{
		Destroy(0.1f);
	}
	MonsterRenderer->ChangeAnimation("DieEffect");
	MonsterRenderer->AddPosition({ 0, -20 });
	DeathCheck = true;
}
void AWaddleDee::Die(float _DeltaTime)
{

	if (IcePlayerBulletCollision == nullptr && true == MonsterRenderer->IsCurAnimationEnd())
	{
		MonsterRenderer->ActiveOff();
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