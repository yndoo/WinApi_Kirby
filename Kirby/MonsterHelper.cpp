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

	// 커비 흡입 충돌체와 몬스터의 충돌 확인
	std::vector<UCollision*> Result;
	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::InhaleCol, Result))
	{
		// 커비쪽으로 당겨지기
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
		// Bullet종류로 공격 받았을 때
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
		case EEnemyState::Die:
			DieStart();
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
	case EEnemyState::Die:
		Die(_DeltaTime);
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

void MonsterHelper::DieStart()
{

}
void MonsterHelper::Die(float _DeltaTime)
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

	switch (MonsterDir)
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

	if (true == IsDamaged)
	{
		//IsDamaged = false;
		return "Damaged" +_Name + DirName;
	}
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

	if (Dir != DirState)	// 방향 변경됐으면 애니메이션 다시
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);
		IsChanged = true;
	}

	return IsChanged;
}

void MonsterHelper::SwitchIsDamaged(float _DeltaTime, float _CoolTime)
{
	SwitchIsDamagedTimer += _DeltaTime;
	if (SwitchIsDamagedTimer >= _CoolTime)
	{
		IsDamaged = !IsDamaged;
		SwitchIsDamagedTimer = 0.f;
	}
}

bool MonsterHelper::IsWall()
{
	FVector CurPos = GetActorLocation();
	Color8Bit FColor = UContentsHelper::ColMapImage->GetColor(CurPos.iX() + 30, CurPos.iY() - 10, Color8Bit::MagentaA);
	Color8Bit BColor = UContentsHelper::ColMapImage->GetColor(CurPos.iX() - 30, CurPos.iY() - 10, Color8Bit::MagentaA);
	if (FColor == Color8Bit::MagentaA || BColor == Color8Bit::MagentaA)
	{
		return true;
	}
	return false;
}