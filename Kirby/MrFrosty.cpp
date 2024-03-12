#include "MrFrosty.h"
#include "ContentsHelper.h"
#include "Player.h"
#include <EngineCore/EngineDebug.h>

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
	MonsterRenderer->SetTransform({ {0,0}, {350, 350} });

	WideCollision = CreateCollision(EKirbyCollisionOrder::MonsterAround);
	WideCollision->SetScale({ 240, 180 });	
	WideCollision->SetPosition({ 0, -90 });
	WideCollision->SetColType(ECollisionType::Rect);

	MonsterCollision = CreateCollision(EKirbyCollisionOrder::Boss);
	MonsterCollision->SetScale({ 80, 100 });
	MonsterCollision->SetPosition({ 0, -50 });
	MonsterCollision->SetColType(ECollisionType::Rect);


	MonsterRenderer->CreateAnimation("Idle_Right", "MrFrostyIdle_Right.png", { 0,1,0,1 }, 0.5f, true);
	MonsterRenderer->CreateAnimation("Idle_Left", "MrFrostyIdle_Left.png", { 0,1,0,1 }, 0.5f, true);
	MonsterRenderer->CreateAnimation("Move_Right", "MrFrostyMove_Right.png", 0, 2, 0.1f, true);
	MonsterRenderer->CreateAnimation("Move_Left", "MrFrostyMove_Left.png", 0, 2, 0.1f, true);
	MonsterRenderer->CreateAnimation("ShootReady", "MF_ShootReady.png", {1, 0, 1, 2}, 0.1f, true);
	MonsterRenderer->CreateAnimation("ShootJump_Right", "MrFrostyShoot_Right.png", 0, 2, 0.1f, false);
	MonsterRenderer->CreateAnimation("ShootJump_Left", "MrFrostyShoot_Left.png", 0, 2, 0.1f, false);
	MonsterRenderer->CreateAnimation("Shoot_Right", "MrFrostyShoot_Right.png", 2, 5, 0.1f, false);
	MonsterRenderer->CreateAnimation("Shoot_Left", "MrFrostyShoot_Left.png", 2, 5, 0.1f, false);

	MonsterRenderer->CreateAnimation("DamagedIdle_Right", "MF_DamagedIdle_Right.png", 0, 1, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedIdle_Left", "MF_DamagedIdle_Left.png", 0, 1, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedMove_Right", "MF_DamagedMove_Right.png", 0, 2, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedMove_Left", "MF_DamagedMove_Left.png", 0, 2, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedShootReady", "MF_DamagedShootReady.png", { 1, 0, 1, 2, 1, 0, 1, 2 }, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedShoot_Right", "MF_DamagedShoot_Right.png", 0, 1, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedShoot_Left", "MF_DamagedShoot_Left.png", 0, 1, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedHitWall_Right", "MF_DamagedHitWall_Right.png", 0, 1, 0.1f, true);
	MonsterRenderer->CreateAnimation("DamagedHitWall_Left", "MF_DamagedHitWall_Left.png", 0, 1, 0.1f, true);

	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
	StateChange(EEnemyState::Idle);
}

void AMrFrosty::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	if (true == IsDamaged)
	{
		SwitchIsDamaged(_DeltaTime, 2.0f);	// 2�ʰ� ����
	}

	int CurHp = GetCurHp();
	if (CurHp <= 0)
	{
		StateChange(EEnemyState::Die);
		return;
	}

	std::vector<UCollision*> Result;
	if (false == IsDamaged && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::PlayerBullet, Result))
	{
		IsDamaged = true;
		AddDamageHp(60);
	}

	// ���� ü�� ������
	//UEngineDebug::DebugTextPrint("Mr.Frosty HP : " + std::to_string(CurHp), 30.0f);
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
	case EEnemyState::Die:
		Die(_DeltaTime);
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
		case EEnemyState::Die:
			DieStart();
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
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));

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
	MonsterRenderer->ChangeAnimation(GetAnimationName("Move"));

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
	float FrostyXPos = GetActorLocation().X;
	if (FrostyXPos < 100.f || FrostyXPos > 570.f)
	{
		StateChange(EEnemyState::HitWall);
		return;
	}
}

void AMrFrosty::HitWallStart()
{
	//�̹����� �� ���� �״��, ������ ƨ�� ������ �� �� �ݴ� ����
	MonsterRenderer->AnimationReset();
	if (false == IsDamaged)
	{
		MonsterRenderer->SetImage(GetAnimationName("HitWall") + std::string(".png"));
	}
	else
	{
		MonsterRenderer->ChangeAnimation(GetAnimationName("HitWall"));
	}

	// ������ ƨ�ܳ�����
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
	if (true == IsDamaged)
	{
		MonsterRenderer->ChangeAnimation(GetAnimationName("HitWall"));
	}

	// ����
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

	// MoveVector�� ���� ���� ���� ����
	if (abs(MoveVector.X) < 80.f)
	{
		MoveVector = FVector::Zero;
		// Ŀ�� ������ ������ ShootReady
		std::vector<UCollision*> Result;
		if (true == WideCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
		{
			StateChange(EEnemyState::ShootReady);
			return;
		}
		// Ŀ�� �ָ� ������ Move 
		// Ŀ�� ������ Throw(�̰� ���߿�)
		StateChange(EEnemyState::Move);
		return;
	}
}

void AMrFrosty::ShootReadyStart()
{
	Timer = 0.f;
	MonsterRenderer->ChangeAnimation(GetAnimationName("ShootReady"));
}
void AMrFrosty::ShootReady(float _DeltaTime)
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("ShootReady"));

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
	// ���� ���� ������ ��.
	bullet = GetWorld()->SpawnActor<AIceBullet>();
	bullet->SetActorLocation(GetActorLocation() + FVector({ 0, -100 }));
	bullet->SetDir(DirState);
}
void AMrFrosty::ShootJump(float _DeltaTime)
{
	// ���Ͷ� IceBullet�̶� �浹�ϸ� ShootStart
	std::vector<UCollision*> Result;
	if (true == bullet->GetDeathCheck() || true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::EdibleBullet, Result))
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
	MonsterRenderer->ChangeAnimation(GetAnimationName("Shoot"));

	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		StateChange(EEnemyState::Idle);
		return;
	}
}

void AMrFrosty::DieStart()
{
	DirCheck();
	MonsterRenderer->ChangeAnimation(GetAnimationName("HitWall"));

	// �ڷ� ƨ�ܳ�����
	JumpVector = SmallJumpPower;
	switch (DirState)
	{
	case EActorDir::Right:
		MoveVector += FVector::Left * 200.f;
		break;
	case EActorDir::Left:
		MoveVector += FVector::Right * 200.f;
		break;
	default:
		break;
	}
}
void AMrFrosty::Die(float _DeltaTime)
{
	float FrostyXPos = GetActorLocation().X;
	if (FrostyXPos < 100.f || FrostyXPos > 570.f)
	{
		MoveVector = FVector::Zero;
	}

	// ����
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


	// MoveVector�� ���� ���� ���� ����
	if (abs(MoveVector.X) < 80.f)
	{
		MoveVector = FVector::Zero;
		if (false == DeathCheck)
		{
			MonsterCollision->Destroy();
			MonsterRenderer->Destroy(3.f);
			//Destroy(5.f);
			DeathCheck = true;
		}
	}

	// 3�� ������ Die����Ʈ ����� �� Destroy? �� ���¸� ����°Գ�����?
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

std::string AMrFrosty::GetAnimationName(std::string _Name)
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

	if (_Name == "ShootReady")
	{
		if (true == IsDamaged)
		{
			return "Damaged" + _Name;
		}
		return _Name;
	}

	if (true == IsDamaged)
	{
		return "Damaged" + _Name + DirName;
	}
	return _Name + DirName;
}