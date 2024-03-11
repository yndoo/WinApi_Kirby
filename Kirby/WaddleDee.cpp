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
	SetMaxHp(100);

	MonsterRenderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	MonsterRenderer->SetImage("WaddleDee_Right.png");
	MonsterRenderer->SetTransform({ {0,0}, {300, 300} });
	MonsterRenderer->SetTransColor(Color8Bit::Magenta);

	MonsterRenderer->CreateAnimation("Idle_Right", "WaddleDee_Right.png", 0, 4, 0.3f, true);
	MonsterRenderer->CreateAnimation("Idle_Left", "WaddleDee_Left.png", 0, 4, 0.3f, true);
	MonsterRenderer->CreateAnimation("Inhaled_Right", "WaddleDee_Right.png", 8, 8, 0.1f, false);
	MonsterRenderer->CreateAnimation("Inhaled_Left", "WaddleDee_Left.png", 8, 8, 0.1f, false);
	MonsterRenderer->CreateAnimation("Damaged_Right", "WaddleDee_Right.png", { 8,8,8,8 }, 0.3f, false);
	MonsterRenderer->CreateAnimation("Damaged_Left", "WaddleDee_Left.png", { 8,8,8,8 }, 0.3f, false);
	//MonsterRenderer->CreateAnimation("Damaged", "WaddleDee_Right.png", { 0,0,0,0,1 }, 0.2f, false);
	//MonsterRenderer->ChangeAnimation("Idle_Left");

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
	FVector ActorPos = GetActorLocation();
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(ActorPos.iX(), ActorPos.iY(), Color8Bit::MagentaA);
	if (Color != Color8Bit::MagentaA)
	{
		FallDown(Color8Bit::MagentaA);
	}

	switch (DirState)
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

void AWaddleDee::DamagedStart()
{
	AddDamageHp(60);
	MonsterCollision->SetActive(true, 2.0f);
	MonsterRenderer->ChangeAnimation(GetAnimationName("Damaged"));
}
void AWaddleDee::Damaged(float _DeltaTime)
{
	if (GetCurHp() <= 0)
	{
		Destroy();
	}
	else if (MonsterRenderer->IsCurAnimationEnd())
	{
		StateChange(EEnemyState::Idle);
		return;
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
		Destroy();
	}
}