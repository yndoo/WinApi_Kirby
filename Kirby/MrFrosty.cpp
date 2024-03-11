#include "MrFrosty.h"
#include "ContentsHelper.h"
#include "Player.h"
#include "IceBullet.h"

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

	WideCollision = CreateCollision(EKirbyCollisionOrder::MonsterAround);
	WideCollision->SetScale({ 240, 160 });	
	WideCollision->SetPosition({ 0, -80 });
	WideCollision->SetColType(ECollisionType::Rect);

	MonsterCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	MonsterCollision->SetScale({ 80, 100 });
	MonsterCollision->SetPosition({ 0, -50 });
	MonsterCollision->SetColType(ECollisionType::Rect);


	MonsterRenderer->CreateAnimation("Idle_Right", "MrFrostyIdle_Right.png", { 0,1,0,1 }, 0.5f, true);
	MonsterRenderer->CreateAnimation("Idle_Left", "MrFrostyIdle_Left.png", { 0,1,0,1 }, 0.5f, true);
	MonsterRenderer->CreateAnimation("Move_Right", "MrFrostyMove_Right.png", 1, 2, 0.1f, true);
	MonsterRenderer->CreateAnimation("Move_Left", "MrFrostyMove_Left.png", 1, 2, 0.1f, true);
	MonsterRenderer->CreateAnimation("ShootReady", "MrFrostyShootReady.png", {1, 0, 1, 2}, 0.1f, true);
	//MonsterRenderer->CreateAnimation("ShootReady", "MrFrostyShootReady.png",  0,2 , 0.1f, true);
	MonsterRenderer->CreateAnimation("ShootJump_Right", "MrFrostyShoot_Right.png", 0, 2, 0.1f, false);
	MonsterRenderer->CreateAnimation("ShootJump_Left", "MrFrostyShoot_Left.png", 0, 2, 0.1f, false);
	MonsterRenderer->CreateAnimation("Shoot_Right", "MrFrostyShoot_Right.png", 2, 5, 0.1f, false);
	MonsterRenderer->CreateAnimation("Shoot_Left", "MrFrostyShoot_Left.png", 2, 5, 0.1f, false);


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
	case EEnemyState::HitWall:
		HitWall(_DeltaTime);
		break;
	case EEnemyState::ShootReady:
		ShootReady(_DeltaTime);
		break;
	case EEnemyState::ShootJump:
		ShootJump(_DeltaTime);
		break;
	case EEnemyState::Shoot:
		Shoot(_DeltaTime);
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
		case EEnemyState::HitWall:
			HitWallStart();
			break;
		case EEnemyState::ShootReady:
			ShootReadyStart();
			break;
		case EEnemyState::ShootJump:
			ShootJumpStart();
			break;
		case EEnemyState::Shoot:
			ShootStart();
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
	MoveVector = FVector::Zero;
	JumpVector = FVector::Zero;
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
}
void AMrFrosty::Idle(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);
	if (true == IsStart)
	{
		IsStart = false;
		StateChange(EEnemyState::Move);
		return;
	}
	if (MonsterRenderer->GetCurAnimation()->Name == UEngineString::ToUpper(GetAnimationName("Idle")) && MonsterRenderer->IsCurAnimationEnd() == true)
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

	// 벽에 닿으면 HitWall 상태로 변경해야함
	float FrostyXPos = GetActorLocation().X;
	if (FrostyXPos < 100.f || FrostyXPos > 570.f)
	{
		StateChange(EEnemyState::HitWall);
		return;
	}
}

void AMrFrosty::HitWallStart()
{
	//이미지는 벽 방향 그대로, 벽에서 튕겨 나오는 건 벽 반대 방향
	MonsterRenderer->AnimationReset();
	MonsterRenderer->SetImage(GetAnimationName("Damaged") + std::string(".png"));
	JumpVector = SmallJumpPower;	
	switch (DirState)
	{
	case EActorDir::Right:
		MoveVector += FVector::Left * 400.f;
		break;
	case EActorDir::Left:
		MoveVector += FVector::Right * 400.f;
		break;
	default:
		break;
	}
}
void AMrFrosty::HitWall(float _DeltaTime)
{
	// 벽에서 튕겨나가기
	switch (DirState)
	{
	case EActorDir::Right:
		AddMoveVector(FVector::Right * _DeltaTime, SmallMoveAcc);
		break;
	case EActorDir::Left:
		AddMoveVector(FVector::Left * _DeltaTime, SmallMoveAcc);
		break;
	default:
		break;
	}
	MoveUpdate(_DeltaTime);

	// MoveVector가 일정 이하 됐을 때는
	if (abs(MoveVector.X) < 80.f)
	{
		// 커비가 가까이 있으면 ShootReady
		std::vector<UCollision*> Result;
		if (true == WideCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
		{
			StateChange(EEnemyState::ShootReady);
			return;
		}
		// 커비가 멀리 있으면 Move 
		// 커비가 닿으면 Throw(이건 나중에)
		MoveVector = FVector::Zero;
		StateChange(EEnemyState::Move);
		return;
	}
}

void AMrFrosty::ShootReadyStart()
{
	Timer = 0.f;
	MonsterRenderer->ChangeAnimation("ShootReady");
}
void AMrFrosty::ShootReady(float _DeltaTime)
{
	Timer += _DeltaTime;
	if (Timer >= ShootReadyCoolTime)
	{
		StateChange(EEnemyState::ShootJump);
		return;
	}
}

void AMrFrosty::ShootJumpStart()
{
	DirCheck();
	MonsterRenderer->ChangeAnimation(GetAnimationName("Move"));
	// 얼음 위로 던져야 함.
	AIceBullet* bullet = GetWorld()->SpawnActor<AIceBullet>();
	bullet->SetActorLocation(GetActorLocation() + FVector({ 0, -100 }));
	bullet->SetDir(DirState);
	bullet->Destroy(3.f);
}
void AMrFrosty::ShootJump(float _DeltaTime)
{
	// 충돌하면 ShootStart
	std::vector<UCollision*> Result;
	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::PlayerBullet, Result))
	{
		StateChange(EEnemyState::Shoot);
		return;
	}
}

void AMrFrosty::ShootStart()
{
	DirCheck();
	MonsterRenderer->ChangeAnimation(GetAnimationName("Shoot"));
}
void AMrFrosty::Shoot(float _DeltaTime)
{
	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		StateChange(EEnemyState::Idle);
		return;
	}
}

void AMrFrosty::MoveUpdate(float _DeltaTime, float MaxSpeed/* = 0.0f*/, FVector Acc /*= FVector::Zero*/)
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

void AMrFrosty::AddMoveVector(const FVector& _DirDelta, FVector Acc)
{
	MoveVector += _DirDelta * Acc;
}

void AMrFrosty::CalMoveVector(float _DeltaTime, float MaxSpeed)
{
	// 최대 속도를 넘어가지 않도록
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

//void AMrFrosty::CalJumpVector(float _DeltaTime)
//{
//	FVector DownVec = FVector::Down * _DeltaTime * 100.f;
//	if (JumpVector.Y + DownVec.Y <= 0)
//	{
//		JumpVector += DownVec;
//	}
//
//}

void AMrFrosty::CalFinalMoveVector(float _DeltaTime)
{
	FinalMoveVector = FVector::Zero;
	FinalMoveVector += MoveVector;
	FinalMoveVector += GravityVector;
	FinalMoveVector += JumpVector;
}

// 최종 계산된 방향과 힘으로 이동시키는 함수
void AMrFrosty::FinalMove(float _DeltaTime)
{
	FVector MovePos = FinalMoveVector * _DeltaTime;
	AddActorLocation(MovePos);
}

bool AMrFrosty::DirCheck()
{
	bool IsChanged = false;
	EActorDir NewDir = EActorDir::None;

	float KirbyXPos = Kirby->GetActorLocation().X;
	float FrostyXPos = GetActorLocation().X;
	float FrostyDir = KirbyXPos - FrostyXPos;
	if (FrostyDir > 0)
	{
		NewDir = EActorDir::Right;
	}
	else if (FrostyDir < 0)
	{
		NewDir = EActorDir::Left;
	}

	if (NewDir != DirState)
	{
		IsChanged = true;
		DirState = NewDir;
	}

	return IsChanged;
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