#include "MonsterHelper.h"
#include "Player.h"

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
	if (false == IsDamaged && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::InhaleCol, Result))
	{
		// 커비쪽으로 당겨지기
		InhaleDir = Result[0]->GetOwner()->GetActorLocation() - GetActorLocation();
		AddActorLocation(InhaleDir.Normalize2DReturn() * InhaleSpeed * _DeltaTime);
		StateChange(EEnemyState::Inhaled);
		return;
	}

	if (true == IsDamaged)
	{
		//SwitchIsDamaged(_DeltaTime, 2.0f);	// 피격 시 2초간 IsDamaged 변수 true, 활용해서 무적 만들기 가능
	}

	if (false == DeathCheck && nullptr != MonsterCollision && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		StateChange(EEnemyState::Damaged);
		return;
	}

	if (false == DeathCheck && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::PlayerBullet, Result))
	{
		// Bullet종류로 공격 받았을 때
		StateChange(EEnemyState::Damaged);
		return;
	}

	if (false == DeathCheck && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::IceBreathBullet, Result))
	{
		// IceBreathBullet 종류로 공격 받았을 때
		IsIced = true;
		StateChange(EEnemyState::Iced);
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
		case EEnemyState::Iced:
			IcedStart();
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
	case EEnemyState::Iced:
		Iced(_DeltaTime);
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

void MonsterHelper::IcedStart()
{
	IcePushed = false; 
	MonsterRenderer->AnimationReset();
	MonsterRenderer->SetImage("IceBlock.png");
	MonsterRenderer->SetScale({ 40, 40 });
	MonsterRenderer->SetPosition({ 0, -20 });

	MonsterCollision->ActiveOff();

	IceCollision = CreateCollision(EKirbyCollisionOrder::IcedMonster);
	IceCollision->SetScale({ 40, 40 });
	IceCollision->SetPosition({ 0, -20 });
	IceCollision->SetColType(ECollisionType::Rect);
	IceCollision->ActiveOn();

	IcePlayerBulletCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);
	IcePlayerBulletCollision->SetScale({ 40, 40 });
	IcePlayerBulletCollision->SetPosition({ 0, -20 });
	IcePlayerBulletCollision->SetColType(ECollisionType::Rect);
	IcePlayerBulletCollision->ActiveOff();
}
void MonsterHelper::Iced(float _DeltaTime)
{
	// 얼음이 된 몬스터를 밀치는 순간 커비의 공격체가 됨
	std::vector<UCollision*> Result;
	if (false == IcePushed && true == IceCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		DirCheck();
		IcePushed = true;
		IceCollision->ActiveOff();

		IcePlayerBulletCollision->ActiveOn();

		MonsterRenderer->SetImage("IceBlock.png");
		MonsterRenderer->SetScale({ 40, 40 });
		MonsterRenderer->SetPosition({ 0, -20 });
	}

	if (true == IcePushed)
	{
		// 커비에게 밀렸다면 그 방향으로 돌진
		switch (DirState)
		{
		case EActorDir::Left:
			AddActorLocation(FVector::Left * 200.f * _DeltaTime);
			break;
		case EActorDir::Right:
			AddActorLocation(FVector::Right * 200.f * _DeltaTime);
			break;
		default:
			break;
		}

		// 몬스터에 닿으면 없애기
		std::vector<UCollision*> Result;
		if (true == IcePlayerBulletCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result))
		{
			//IceCollision->Destroy();
			//IceCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);	// 잔상용
			//IceCollision->SetScale({ 40, 40 });
			//IceCollision->SetPosition({ 0, -20 });
			//IceCollision->SetColType(ECollisionType::Rect);
			//IceCollision->ActiveOn();

			//IcePlayerBulletCollision->ActiveOff();
			MonsterRenderer->ActiveOff();

			StateChange(EEnemyState::Die);
			return;
		}
		return;
	}
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
		return "Damaged" + DirName;
	}
	return _Name + DirName;
}

bool MonsterHelper::DirCheck()
{
	bool IsChanged = false;
	EActorDir Dir = DirState;

	Dir = Kirby->GetKirbyDir();

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