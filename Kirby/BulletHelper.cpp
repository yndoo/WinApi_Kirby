#include "BulletHelper.h"

BulletHelper::BulletHelper()
{
}

BulletHelper::~BulletHelper()
{
}

void BulletHelper::BeginPlay()
{
	AActor::BeginPlay();
}

void BulletHelper::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void  BulletHelper::StateChange(EBulletState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EBulletState::Idle:
			IdleStart();
			break;
		case EBulletState::Damage:
			DamageStart();
			break;
		case EBulletState::Finish:
			FinishStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void  BulletHelper::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EBulletState::Idle:
		Idle(_DeltaTime);
		break;
	case EBulletState::Damage:
		Damage(_DeltaTime);
		break;
	case EBulletState::Finish:
		Finish(_DeltaTime);
		break;
	default:
		break;
	}
}

void BulletHelper::IdleStart()
{
	// Bullet�� �� �� �� �������� ���� �ǹǷ� �� �� �޾ƿ� Dir�� �����. (== DirCheck �ʿ� ����)
	BulletRenderer->ChangeAnimation(GetAnimationName("Idle"));
}

void BulletHelper::DamageStart()
{

}

void BulletHelper::FinishStart()
{

}

void BulletHelper::Idle(float _DeltaTime)
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

	// ������ Monster�� ������
	std::vector<UCollision*> Result;
	if (true == BulletCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result))
	{
		AActor* temp = Result[0]->GetOwner();
		int a = 0;

		StateChange(EBulletState::Damage);
		return;
	}
}

void BulletHelper::Damage(float _DeltaTime)
{
	// ������ �ִϸ��̼� �ߴ� ġ��~
	// true == IsCurAnimationEnd() ��
	Destroy();
}

void BulletHelper::Finish(float _DeltaTime)
{

}


std::string BulletHelper::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (Dir)
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

	return BulletName + _Name + DirName;
}