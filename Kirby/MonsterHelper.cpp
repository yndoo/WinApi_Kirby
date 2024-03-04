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