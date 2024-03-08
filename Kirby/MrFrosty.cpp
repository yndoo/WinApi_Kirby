#include "MrFrosty.h"
#include "ContentsHelper.h"
#include "Player.h"

AMrFrosty::AMrFrosty()
{
}

AMrFrosty::~AMrFrosty()
{
}

void AMrFrosty::BeginPlay()
{
	MonsterRenderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	MonsterRenderer->SetImage("MrFrostyIdle_Left.png");
	MonsterRenderer->SetTransform({ {0,0}, {300, 300} });

	MonsterCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	MonsterCollision->SetScale({ 40, 40 });
	MonsterCollision->SetPosition({ 0, -20 });
	MonsterCollision->SetColType(ECollisionType::Rect);

	MonsterRenderer->CreateAnimation("Idle_Right", "MrFrostyIdle_Right.png", { 0,1,0,1 }, 0.5f, true);
	MonsterRenderer->CreateAnimation("Idle_Left", "MrFrostyIdle_Left.png", { 0,1,0,1 }, 0.5f, true);
	MonsterRenderer->CreateAnimation("Move_Right", "MrFrostyMove_Right.png", 1, 2, 0.1f, true);
	MonsterRenderer->CreateAnimation("Move_Left", "MrFrostyMove_Left.png", 1, 2, 0.1f, true);

	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
	StateChange(EEnemyState::Idle);
}

void AMrFrosty::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void AMrFrosty::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EEnemyState::Idle:
		Idle(_DeltaTime);
		break;
	case EEnemyState::Move:
		Move(_DeltaTime);
		break;
	default:
		break;
	}
}

void AMrFrosty::StateChange(EEnemyState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EEnemyState::Idle:
			IdleStart();
			break;
		case EEnemyState::Move:
			MoveStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void AMrFrosty::IdleStart()
{
	DirCheck();
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
}
void AMrFrosty::Idle(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);
	if (true == IsStart)
	{
		StateChange(EEnemyState::Move);
		return;
	}
}

void AMrFrosty::MoveStart()
{
	DirCheck();
	MonsterRenderer->ChangeAnimation(GetAnimationName("Move"));
	JumpVector = SmallJumpPower;
}
void AMrFrosty::Move(float _DeltaTime)
{
	float KirbyXPos = Kirby->GetActorLocation().X;
	float FrostyXPos = GetActorLocation().X;
	float FrostyDir = KirbyXPos - FrostyXPos;
	if (FrostyDir > 0)
	{
		DirState = EActorDir::Right;
	}
	else if (FrostyDir < 0)
	{
		DirState = EActorDir::Left;
	}

	switch (DirState)
	{
	case EActorDir::Right:
		AddMoveVector(FVector::Right * _DeltaTime, MoveAcc);
		break;
	case EActorDir::Left:
		AddMoveVector(FVector::Left * _DeltaTime, MoveAcc);
		break;
	default:
		break;
	}
	MoveUpdate(_DeltaTime);
		
	// ���� ������ HitWall ���·� �����ؾ���
	FrostyXPos = GetActorLocation().X;
	if (FrostyXPos < 100.f)
	{
		int a = 0;
	}

}

void AMrFrosty::MoveUpdate(float _DeltaTime, float MaxSpeed/* = 0.0f*/, FVector Acc /*= FVector::Zero*/)
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

void AMrFrosty::AddMoveVector(const FVector& _DirDelta, FVector Acc)
{
	MoveVector += _DirDelta * Acc;
}

void AMrFrosty::CalMoveVector(float _DeltaTime, float MaxSpeed)
{
	// �� ������ üũ
	FVector CheckPos = GetActorLocation();
	CheckPos.X += static_cast<float>(DirState) * 20.0f;	// �յڷ� 20�ȼ�
	CheckPos.Y -= 20;									// �ܵ� ��� ������

	FVector CurPos = GetActorLocation();
	CurPos.X += static_cast<float>(DirState) * 20.0f;	// �յڷ� 20�ȼ�
	CurPos.Y -= 28;										// ���δ� �ö�ߵ�

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	Color8Bit Color2 = UContentsHelper::ColMapImage->GetColor(CurPos.iX(), CurPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA && Color2 == Color8Bit::MagentaA)
	{
		MoveVector = FVector::Zero;
	}

	// �ִ� �ӵ��� �Ѿ�� �ʵ���
	if (MaxSpeed <= MoveVector.Size2D())
	{
		MoveVector = MoveVector.Normalize2DReturn() * MaxSpeed;
	}
}

void AMrFrosty::CalGravityVector(float _DeltaTime)
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

void AMrFrosty::CalJumpVector(float _DeltaTime)
{
	FVector DownVec = FVector::Down * _DeltaTime * 100.f;
	if (JumpVector.Y - DownVec.Y <= 0)
	{
		JumpVector += DownVec;
	}
}

void AMrFrosty::CalFinalMoveVector(float _DeltaTime)
{
	FinalMoveVector = FVector::Zero;
	FinalMoveVector += MoveVector;
	FinalMoveVector += GravityVector;
	FinalMoveVector += JumpVector;
}

// ���� ���� ����� ������ �̵���Ű�� �Լ�
void AMrFrosty::FinalMove(float _DeltaTime)
{
	MapSize = UContentsHelper::ColMapImage->GetScale();

	FVector MovePos = FinalMoveVector * _DeltaTime;					// �÷��̾� �̵��� (�ȱ��� Move�� �ƴ�)

	FVector PrevPlayerPos = GetActorLocation();
	FVector NextPlayerPos = PrevPlayerPos + MovePos;

	// �̵�
	AddActorLocation(MovePos);
}

bool AMrFrosty::IsPlayerBottomMagentaA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA)
	{
		return true;
	}
	return false;
}

bool AMrFrosty::IsPlayerBottomYellowA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::YellowA)
	{
		return true;
	}
	return false;
}

void AMrFrosty::UpMoving(float _DeltaTime, Color8Bit _Color)
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