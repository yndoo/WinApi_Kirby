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

	BulletCollision = CreateCollision(EKirbyCollisionOrder::PlayerBullet);
	BulletCollision->SetScale({ 40, 40 });
	BulletCollision->SetPosition({ 0, -20 });
	BulletCollision->SetColType(ECollisionType::Rect);

	StateChange(EBulletState::Idle);
}

void AIceBullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void AIceBullet::IdleStart()
{
	JumpVector = JumpPower;
}
void AIceBullet::Idle(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);
	// ���� �������ٰ�

	// ���Ͷ� �浹�ϸ� ������ ������
	std::vector<UCollision*> Result;
	if (true == BulletCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result))
	{
		StateChange(EBulletState::Move);
	}
}

void AIceBullet::MoveStart()
{

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

	JumpVector += FVector::Down * _DeltaTime * 100.f;
}

void AIceBullet::MoveUpdate(float _DeltaTime, float MaxSpeed/* = 0.0f*/, FVector Acc /*= FVector::Zero*/)
{
	// MaxSpeed, Acc �Է� �������� CalMoveVector �ϸ� �� ��
	if (MaxSpeed != 0.0f)
	{
		CalMoveVector(_DeltaTime, MaxSpeed);
	}
	CalGravityVector(_DeltaTime);
	CalFinalMoveVector(_DeltaTime);
	FinalMove(_DeltaTime);

	// �ٴڿ� ������ �ʵ��� �÷���
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
	// �ִ� �ӵ��� �Ѿ�� �ʵ���
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

// ���� ���� ����� ������ �̵���Ű�� �Լ�
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