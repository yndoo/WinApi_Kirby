#include "MonsterHelper.h"

MonsterHelper::MonsterHelper()
{
}

MonsterHelper::~MonsterHelper()
{
}

void MonsterHelper::BeginPlay()
{
	AActor::BeginPlay();
}

void MonsterHelper::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	// Ŀ�� ���� �浹ü�� ������ �浹 Ȯ��
	std::vector<UCollision*> Result;
	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::InhaleCol, Result))
	{
		// Ŀ�������� �������
		InhaleDir = Result[0]->GetOwner()->GetActorLocation() - GetActorLocation();
		//FVector test = InhaleDir.Normalize2DReturn();
		AddActorLocation(InhaleDir.Normalize2DReturn() * 100.f * _DeltaTime);
		StateChange(EEnemyState::Inhaled);
		return;
	}

	if (nullptr != MonsterCollision && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		StateChange(EEnemyState::Damaged);
		return;
	}

	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::PlayerBullet, Result))
	{
		// Bullet������ ���� �޾��� ��
		StateChange(EEnemyState::Damaged);
		return;
	}
}

void MonsterHelper::StateChange(EEnemyState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EEnemyState::Idle:
			IdleStart();
			break;
		case EEnemyState::Damaged:
			DamagedStart();
			break;
		case EEnemyState::Move:
			MoveStart();
			break;
		case EEnemyState::Inhaled:
			InhaledStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void MonsterHelper::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EEnemyState::Idle:
		Idle(_DeltaTime);
		break;
	case EEnemyState::Damaged:
		Damaged(_DeltaTime);
		break;
	case EEnemyState::Move:
		Move(_DeltaTime);
		break;
	case EEnemyState::Inhaled:
		Inhaled(_DeltaTime);
		break;
	default:
		break;
	}
}

void MonsterHelper::IdleStart()
{

}

void MonsterHelper::Idle(float _DeltaTime)
{

}

void MonsterHelper::DamagedStart()
{

}

void MonsterHelper::Damaged(float _DeltaTime)
{

}

void MonsterHelper::MoveStart()
{

}

void MonsterHelper::Move(float _DeltaTime)
{

}

void MonsterHelper::InhaledStart()
{

}

void MonsterHelper::Inhaled(float _DeltaTime)
{

}

void MonsterHelper::FallDown(Color8Bit _Color)
{
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color != _Color)
		{
			AddActorLocation(FVector::Down);
		}
		else
		{
			break;
		}
	}
}

std::string MonsterHelper::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (DirState)
	{
	case EActorDir::Left:
		DirName = "_Left";
		break;
	case EActorDir::Right:
		DirName = "_Right";
		break;
	default:
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;
}

bool MonsterHelper::DirCheck()
{
	bool IsChanged = false;
	EActorDir Dir = DirState;
	if (UEngineInput::IsPress(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		Dir = EActorDir::Right;
	}

	if (Dir != DirState)	// ���� ��������� �ִϸ��̼� �ٽ�
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);
		IsChanged = true;
	}

	return IsChanged;
}