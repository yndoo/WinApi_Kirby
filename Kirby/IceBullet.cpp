#include "IceBullet.h"

AIceBullet::AIceBullet()
{
}

AIceBullet::~AIceBullet()
{
}

void AIceBullet::BeginPlay()
{
	BulletRenderer = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer->SetImage("IceBlock.png");
	BulletRenderer->SetTransform({ {0,-20}, {40, 40} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);

	BulletCollision = CreateCollision(EKirbyCollisionOrder::EdibleBullet);
	BulletCollision->SetScale({ 40, 40 });
	BulletCollision->SetPosition({ 0, -20 });
	BulletCollision->SetColType(ECollisionType::Rect);

	StateChange(EBulletState::Idle);
}

void AIceBullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	// 커비 흡입 충돌체와 몬스터의 충돌 확인
	std::vector<UCollision*> Result;
	if (true == BulletCollision->CollisionCheck(EKirbyCollisionOrder::InhaleCol, Result))
	{
		// 커비쪽으로 당겨지기
		FVector InhaleDir = Result[0]->GetOwner()->GetActorLocation() - GetActorLocation();
		AddActorLocation(InhaleDir.Normalize2DReturn() * 100.f * _DeltaTime);
		return;
	}

	if (nullptr != BulletCollision && true == BulletCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		Destroy();
	}
}

void AIceBullet::IdleStart()
{
	// 위로 던져졌다가
	JumpVector = JumpPower;
}
void AIceBullet::Idle(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);

	// 몬스터랑 충돌하면 앞으로 포물선
	std::vector<UCollision*> Result;
	if (true == BulletCollision->CollisionCheck(EKirbyCollisionOrder::Boss, Result))
	{
		StateChange(EBulletState::Move);
	}
}

void AIceBullet::MoveStart()
{
	GravityVector = FVector::Zero;
	switch (Dir)
	{
	case EActorDir::Left:
		MoveVector += FVector::Left * 100.f;
		break;
	case EActorDir::Right:
		MoveVector += FVector::Right * 100.f;
		break;
	}
}
void AIceBullet::Move(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);
	switch (Dir)
	{
	case EActorDir::Left:
		AddMoveVector(FVector::Left * _DeltaTime, MoveAcc);
		break;
	case EActorDir::Right:
		AddMoveVector(FVector::Right * _DeltaTime, MoveAcc);
		break;
	}

	JumpVector += FVector::Down * _DeltaTime * 200.f;
}

void AIceBullet::MoveUpdate(float _DeltaTime, float MaxSpeed/* = 0.0f*/, FVector Acc /*= FVector::Zero*/)
{
	// MaxSpeed, Acc 입력 없었으면 CalMoveVector 하면 안 됨
	if (MaxSpeed != 0.0f)
	{
		CalMoveVector(_DeltaTime, MaxSpeed);
	}
	CalGravityVector(_DeltaTime);
	CalFinalMoveVector(_DeltaTime);
	FinalMove(_DeltaTime);

	// 바닥에 박히지 않도록 올려줌
	if (IsPlayerBottomMagentaA())
	{
		UpMoving(_DeltaTime, Color8Bit::MagentaA);
	}
	if (IsPlayerBottomYellowA())
	{
		UpMoving(_DeltaTime, Color8Bit::YellowA);
	}
}

void AIceBullet::AddMoveVector(const FVector& _DirDelta, FVector Acc)
{
	MoveVector += _DirDelta * Acc;
}

void AIceBullet::CalMoveVector(float _DeltaTime, float MaxSpeed)
{
	// 최대 속도를 넘어가지 않도록
	if (MaxSpeed <= MoveVector.Size2D())
	{
		MoveVector = MoveVector.Normalize2DReturn() * MaxSpeed;
	}
}

void AIceBullet::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime;

	if (IsPlayerBottomMagentaA())
	{
		GravityVector = FVector::Zero;
	}

	if (IsPlayerBottomYellowA())
	{
		GravityVector = FVector::Zero;
	}
}

void AIceBullet::CalFinalMoveVector(float _DeltaTime)
{
	FinalMoveVector = FVector::Zero;
	FinalMoveVector += MoveVector;
	FinalMoveVector += GravityVector;
	FinalMoveVector += JumpVector;
}

// 최종 계산된 방향과 힘으로 이동시키는 함수
void AIceBullet::FinalMove(float _DeltaTime)
{
	FVector MovePos = FinalMoveVector * _DeltaTime;
	AddActorLocation(MovePos);
}

bool AIceBullet::IsPlayerBottomMagentaA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA)
	{
		return true;
	}
	return false;
}

bool AIceBullet::IsPlayerBottomYellowA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::YellowA)
	{
		return true;
	}
	return false;
}

void AIceBullet::UpMoving(float _DeltaTime, Color8Bit _Color)
{
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color == _Color)
		{
			AddActorLocation(FVector::Up);
		}
		else
		{
			break;
		}
	}
}