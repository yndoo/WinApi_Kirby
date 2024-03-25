#include "Player.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineDebug.h>
#include "StarBullet.h"
#include "FireBullet.h"
#include "IceBreathBullet.h"

APlayer* Kirby = nullptr;

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::AutoCreateAnimation(std::string_view _AnimationName, std::vector<int> _Indexs, float _Inter, bool _Loop)
{
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Right"), std::string(_AnimationName) + std::string("_Right.png"), _Indexs, _Inter, _Loop);
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Left"), std::string(_AnimationName) + std::string("_Left.png"), _Indexs, _Inter, _Loop);
}
void APlayer::AutoCreateAnimation(std::string_view _AnimationName, int _Start, int _End, float _Inter, bool _Loop)
{
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Right"), std::string(_AnimationName) + std::string("_Right.png"), _Start, _End, _Inter, _Loop);
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Left"), std::string(_AnimationName) + std::string("_Left.png"), _Start, _End, _Inter, _Loop);
}
void APlayer::AutoCreateAnimation(std::string_view _AnimationName, std::string_view _ImageName, std::vector<int> _Indexs, float _Inter, bool _Loop)
{
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Right"), std::string(_ImageName) + std::string("_Right.png"), _Indexs, _Inter, _Loop);
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Left"), std::string(_ImageName) + std::string("_Left.png"), _Indexs, _Inter, _Loop);
}
void APlayer::AutoCreateAnimation(std::string_view _AnimationName, std::string_view _ImageName, int _Start, int _End, float _Inter, bool _Loop)
{
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Right"), std::string(_ImageName) + std::string("_Right.png"), _Start, _End, _Inter, _Loop);
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Left"), std::string(_ImageName) + std::string("_Left.png"), _Start, _End, _Inter, _Loop);
}

// ���ο� Ŀ��� ���¸� ī�����ִ� �Լ�.
// ���� Ŀ�� ��ü�� ���� Kirby�� �ٲٱ� ���� ������Ѿ� ��.
void APlayer::KirbyCopy()
{
	BeforePos = Kirby->BeforePos;
	/*IsEating = Kirby->IsEating;
	IsFireKirby = Kirby->IsFireKirby;
	IsIceKirby = Kirby->IsIceKirby;*/
	EatingFireType = UContentsHelper::EatingFireMonster;
	CurHp = Kirby->CurHp;
	LifeNum = Kirby->LifeNum;
	DamagePower = Kirby->DamagePower;
}

void APlayer::KirbyTypeChange(EKirbyType _KirbyType)
{
	if (UContentsHelper::KirbyType != _KirbyType)
	{
		//KirbyType = _KirbyType;
		UContentsHelper::KirbyType = _KirbyType;
	}
}

void APlayer::BeginPlay() 
{
	AActor::BeginPlay();
	KirbyTypeChange(UContentsHelper::KirbyType);

 	if (nullptr != Kirby)
	{
		// Ŀ�� �̹� �ִ� ���
		KirbyCopy();
		Kirby->Destroy();

		UContentsHelper::EatingFireMonster = EatingFireType;

		std::string LevelName = GetWorld()->GetName();

		if (LevelName == "RESTAREALEVEL")
		{
			SetActorLocation({ 60, 300 });
		}
		else if (LevelName == "BOSSLEVEL")
		{
			SetActorLocation({ 100, 1050 });
		}
		else
		{
			// ���� ����(���� ����� ���δ� PlayLevel�ۿ� ����)�� ���ƿ� ��쿡 �ش�
			SetActorLocation(BeforePos);
		}
	}
	else
	{
		// Ŀ�� �� ���� ����
		// �׽�Ʈ�� ������������ Ŀ�� ���ʻ����ϴ� ���
		std::string LevelName = GetWorld()->GetName();
		if (LevelName == "BOSSLEVEL")
		{
			SetActorLocation({ 100, 1050 });
		}
		else	// Ŀ�� �� ���� ����
		{
			SetActorLocation({ 100, 100 });
			SetKirbyLife(2);
		}

	}

	// ���� Ŀ�� �Ѱ��ֱ�
	Kirby = this;

	// Ŀ�� ������
	PlayerRenderer = CreateImageRenderer(EKirbyRenderOrder::Player);
	PlayerRenderer->SetImage("Kirby.png");
	PlayerRenderer->SetTransform({ {0,0}, {300, 300} });
	
	PlayerRenderer->SetTransColor(Color8Bit::Magenta);

	AutoCreateAnimation("Move", { 0,1,1,2,3,4,5,6,6,7,8,9 }, 0.07f, true);
	AutoCreateAnimation("Idle", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2 }, 0.09f, true);
	AutoCreateAnimation("Crouch", 1, 1, 0.06f, false);
	AutoCreateAnimation("Slide", 0, 0, 0.3f, true);
	AutoCreateAnimation("Run", 0, 7, 0.05f, true);
	AutoCreateAnimation("Brake", 0, 0, 0.2f, false);
	AutoCreateAnimation("Swallow", 0, 4, 0.1f, false);
	AutoCreateAnimation("InhaleStart", "Inhale", 4, 4, 0.1f, false);
	AutoCreateAnimation("InhaleSmall", "Inhale", 5, 6, 0.1f, true);
	AutoCreateAnimation("InhaleLarge", "Inhale", 7, 8, 0.1f, true);
	AutoCreateAnimation("InhaleFail", "Inhale", 9, 12, 0.1f, false);
	AutoCreateAnimation("JumpStart", "Jump", 0, 0, 0.1f, false);
	AutoCreateAnimation("JumpTurn", "Jump", 1, 6, 0.04f, false);
	AutoCreateAnimation("JumpEnd", "Jump", 7, 8, 0.05f, false);
	AutoCreateAnimation("JumpCrouch", "Crouch", 1, 1, 0.06f, false);
	AutoCreateAnimation("FlyStart", "Fly", 0, 4, 0.05f, false);
	AutoCreateAnimation("Flying", "Fly", 5, 9, 0.1f, true);
	AutoCreateAnimation("Exhale", "Fly", { 2, 1, 0 }, 0.15f, false);
	AutoCreateAnimation("Damaged", 0, 8, 0.05f, false);

	AutoCreateAnimation("EatingAttack", 0, 4, 0.1f, false);
	AutoCreateAnimation("EatingEating", "Eating", 2, 6, 0.1f, false);
	AutoCreateAnimation("EatingIdle", "Eating", 6, 6, 0.1f, false);
	AutoCreateAnimation("EatingRun", "EatingMove", 0, 14, 0.05f, true);
	AutoCreateAnimation("EatingMove", "EatingMove", 0, 14, 0.07f, true);
	AutoCreateAnimation("EatingJumpStart", "EatingJump", {0,1,2,3,4,5,6}, 0.08f, false);
	AutoCreateAnimation("EatingJumpEnd", "EatingJump", 7, 7, 0.1f, false);

	AutoCreateAnimation("FireIdle", { 0,1,2,3,0,1,2,3,0,1,2,3, 4, 5 }, 0.09f, true);
	AutoCreateAnimation("FireMove", 0, 19, 0.04f, true);
	AutoCreateAnimation("FireSlide", 0, 4, 0.06f, false);
	AutoCreateAnimation("FireBrake", 0, 1, 0.1f, false);
	AutoCreateAnimation("FireRun", 0, 3, 0.05f, true);
	AutoCreateAnimation("FireCrouch", 0, 1, 0.03f, true);
	AutoCreateAnimation("FireAttack", 0, 3, 0.05f, true);
	AutoCreateAnimation("FireJumpStart", "FireJump", 0, 1, 0.1f, false);
	AutoCreateAnimation("FireJumpTurn", "FireJump", 2, 8, 0.02f, false);
	AutoCreateAnimation("FireJumpEnd", "FireJump", 8, 8, 0.05f, false);
	AutoCreateAnimation("FireJumpCrouch", "FireJump", 10, 10, 0.06f, false);
	AutoCreateAnimation("FireFlyStart", 2, 4, 0.05f, false);
	AutoCreateAnimation("FireFlying", 0, 14, 0.1f, true);
	AutoCreateAnimation("FireExhale", 0, 2, 0.15f, false);

	AutoCreateAnimation("IceIdle", { 0, 0, 0, 0, 0, 0, 0, 1 }, 0.1f, true);
	AutoCreateAnimation("IceMove", 0, 9, 0.1f, true);
	AutoCreateAnimation("IceSlide", 0, 1, 0.12f, false);
	AutoCreateAnimation("IceBrake", 0, 0, 0.1f, false);
	AutoCreateAnimation("IceRun", 0, 7, 0.05f, true); 
	AutoCreateAnimation("IceCrouch", 1, 1, 0.03f, false);
	AutoCreateAnimation("IceAttack", 0, 7, 0.05f, false);
	AutoCreateAnimation("IceJumpStart", "IceJump", 0, 0, 0.1f, false);
	AutoCreateAnimation("IceJumpTurn", "IceJump", 1, 6, 0.02f, false);
	AutoCreateAnimation("IceJumpEnd", "IceJump", 5, 6, 0.05f, false);
	AutoCreateAnimation("IceJumpCrouch", "IceJump", 9, 9, 0.06f, false);
	AutoCreateAnimation("IceFlyStart", 0, 4, 0.05f, false);
	AutoCreateAnimation("IceFlying", 0, 7, 0.1f, true);
	AutoCreateAnimation("IceExhale", 0, 1, 0.15f, false);

	PlayerRenderer->CreateAnimation("LadderUp", "LadderMove.png", 0, 9, 0.1f, true);
	PlayerRenderer->CreateAnimation("LadderDown", "LadderMove.png", 10, 12, 0.2f, true);
	PlayerRenderer->CreateAnimation("FireLadderUp", "FireLadderUp.png", 0, 39, 0.025f, true);
	PlayerRenderer->CreateAnimation("FireLadderDown", "FireLadderDown.png", 0, 15, 0.05f, true);
	PlayerRenderer->CreateAnimation("IceLadderUp", "IceLadderUp.png", 0, 9, 0.1f, true);
	PlayerRenderer->CreateAnimation("IceLadderDown", "IceLadderDown.png", 0, 3, 0.2f, true);

	PlayerRenderer->ChangeAnimation(GetAnimationName("Idle"));

	// Ŀ�� ����
	BodyCollision = CreateCollision(EKirbyCollisionOrder::Player);
	BodyCollision->SetScale({ 40, 0 });
	BodyCollision->SetPosition({ 0, -20 });
	BodyCollision->SetColType(ECollisionType::CirCle);

	// Ŀ�� ���� �浹ü (Ŀ�� �浹�ÿ� �ڷ� �о��� �� ���Ϳ� ���� �浹�ϱ� ���� �Ͻ������� �ʿ�)
	FrontCollision = CreateCollision(EKirbyCollisionOrder::Player);
	FrontCollision->SetScale({ 39, 0 });
	FrontCollision->SetPosition({ 0, -20 });
	FrontCollision->SetColType(ECollisionType::CirCle);
	FrontCollision->ActiveOff();

	// Ŀ�� �ٴ�
	BottomCollision = CreateCollision(EKirbyCollisionOrder::Player);
	BottomCollision->SetScale({ 5, 5 });
	BottomCollision->SetPosition({ 0, 5 });
	BottomCollision->SetColType(ECollisionType::Rect);

	RealBottomCollision = CreateCollision(EKirbyCollisionOrder::Player);
	RealBottomCollision->SetScale({ 10, 2 });
	RealBottomCollision->SetPosition({ 0, 0 });
	RealBottomCollision->SetColType(ECollisionType::Rect);

	// Ŀ�� ���� �浹ü
	InhaleCollision = CreateCollision(EKirbyCollisionOrder::InhaleCol);
	InhaleCollision->SetScale({ 100, 0 });		// ���� �浹ü ũ��� ���� �Է� �ð��� ���� �޶�����.
	InhaleCollision->SetPosition({ 0, -20 });	// ���� �浹ü ��ġ�� ���� �ø��� �ٲ��� �Ѵ�.
	InhaleCollision->SetColType(ECollisionType::Rect);
	InhaleCollision->ActiveOff();

	StateChange(EKirbyState::Idle);
	UContentsHelper::EatingFireMonster = false;
}

void APlayer::Tick(float _DeltaTime) 
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('0'))
	{
		if (DamagePower == 0)
		{
			DamagePower = 80;
		}
		else
		{
			DamagePower = 0;
		}
	}

	std::vector<UCollision*> Result;
	if (State != EKirbyState::Eating &&
		State != EKirbyState::Inhale &&
		(
			true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result) ||
			true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::Boss, Result) ||
			true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::EdibleBullet, Result) ||
			true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::Obstacle, Result)
		)
	   )
	{
		KirbyTypeChange(EKirbyType::Normal);
		BeforeState = State;
		StateChange(EKirbyState::Damaged);
		return;
		
	}

	if (true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::LifeItem, Result))
	{
		LifeNum++;
	}
	if (true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::IceTypeItem, Result))
	{
		KirbyTypeChange(EKirbyType::Ice);
		StateChange(EKirbyState::Changing);
		return;
	}
	if (true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::FireTypeItem, Result))
	{
		KirbyTypeChange(EKirbyType::Fire);
		StateChange(EKirbyState::Changing);
		return;
	}
	if (true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::IcedMonster, Result))
	{
		MoveVector.X = 0;
		return;
	}

	FVector PlayerPos = GetActorLocation();
	UEngineDebug::DebugTextPrint("X : " + std::to_string(PlayerPos.X) + ", Y : " + std::to_string(PlayerPos.Y), 30.0f);
}

void APlayer::StateChange(EKirbyState _State) 
{
	if (State != _State)
	{
		switch (_State)
		{
		case EKirbyState::Idle:
			IdleStart();
			break;
		case EKirbyState::Move:
			MoveStart();
			break;
		case EKirbyState::Run:
			RunStart();
			break;
		case EKirbyState::Slide:
			SlideStart();
			break;
		case EKirbyState::Crouch:
			CrouchStart();
			break;
		case EKirbyState::Jump:
			JumpStart();
			break;
		case EKirbyState::Brake:
			BrakeStart();
			break;
		case EKirbyState::Inhale:
			InhaleStart();
			break;
		case EKirbyState::Eating:
			EatingStart();
			break;
		case EKirbyState::Swallow:
			SwallowStart();
			break;
		case EKirbyState::Attack:
			AttackStart();
			break;
		case EKirbyState::Fly:
			FlyStart();
			break;
		case EKirbyState::LadderUp:
			LadderUpStart();
			break;
		case EKirbyState::LadderDown:
			LadderDownStart();
			break;
		case EKirbyState::Exhale:
			ExhaleStart();
			break;
		case EKirbyState::Damaged:
			DamagedStart();
			break;
		case EKirbyState::InhaleFail:
			InhaleFailStart();
			break;
		case EKirbyState::Changing:
			ChangingStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void APlayer::StateUpdate(float _DeltaTime) {
	switch (State) {
	case EKirbyState::Idle:
		// ������
		Idle(_DeltaTime);
		break;
	case EKirbyState::Move:
		// �̵�
		Move(_DeltaTime);
		break;
	case EKirbyState::Crouch:
		// ��ũ����
		Crouch(_DeltaTime);
		break;
	case EKirbyState::Slide:
		// �����̵�
		Slide(_DeltaTime);
		break;
	case EKirbyState::Run:
		// �޸���
		Run(_DeltaTime);
		break;
	case EKirbyState::Jump:
		// ����
		Jump(_DeltaTime);
		break;
	case EKirbyState::Brake:
		// �극��ũ
		Brake(_DeltaTime);
		break;
	case EKirbyState::Inhale:
		// ����
		Inhale(_DeltaTime);
		break;
	case EKirbyState::Eating:
		// �Դ� ��
		Eating(_DeltaTime);
		break;
	case EKirbyState::Swallow:
		// ��Ű��
		Swallow(_DeltaTime);
		break;
	case EKirbyState::Attack:
		// ����
		Attack(_DeltaTime);
		break;
	case EKirbyState::Fly:
		// ����
		Fly(_DeltaTime);
		break;
	case EKirbyState::LadderUp:
		// ��ٸ� ������
		LadderUp(_DeltaTime);
		break;
	case EKirbyState::LadderDown:
		// ��ٸ� ��������
		LadderDown(_DeltaTime);
		break;
	case EKirbyState::Exhale:
		// ǳ���� �� ���� ���
		Exhale(_DeltaTime);
		break;
	case EKirbyState::Damaged:
		// �ǰ�
		Damaged(_DeltaTime);
		break;
	case EKirbyState::InhaleFail:
		// ���� ����
		InhaleFail(_DeltaTime);
		break;
	case EKirbyState::Changing:
		// ���� ��
		Changing(_DeltaTime);
		break;
	case EKirbyState::FreeMove:
		// ���� �̵�
		FreeMove(_DeltaTime);
		break;
	case EKirbyState::CameraFreeMove:
		// ī�޶� ���� �̵�
		CameraFreeMove(_DeltaTime);
		break;
	default:
		break;
	}
}

// Idle : ������ �ִ� ����
void APlayer::IdleStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Idle"));
	JumpVector = FVector::Zero;
}
void  APlayer::Idle(float _DeltaTime)
{
	MoveVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDoubleClick(VK_RIGHT, 0.3f))
	{
		StateChange(EKirbyState::Run);
		return;
	}
	if (true == UEngineInput::IsDoubleClick(VK_LEFT, 0.3f))
	{
		StateChange(EKirbyState::Run);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
	{
		// �׳� Move
		StateChange(EKirbyState::Move);
		return;
	}

	if (true == UEngineInput::IsPress(VK_UP))
	{
		std::vector<UCollision*> Result;
		if (UContentsHelper::KirbyType != EKirbyType::Eating && true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::Ladder, Result))
		{
			// ��ٸ� ������
			LadderTop = Result[0]->GetOwner()->GetActorLocation().Y - Result[0]->GetTransform().GetScale().hY() - 1.f;
			LadderBottom = Result[0]->GetOwner()->GetActorLocation().Y + Result[0]->GetTransform().GetScale().hY() - 1.f;
			StateChange(EKirbyState::LadderUp);
			return;
		}
	}

	if (true == UEngineInput::IsPress(VK_DOWN))
	{
		std::vector<UCollision*> Result;
		if (UContentsHelper::KirbyType != EKirbyType::Eating && true == BottomCollision->CollisionCheck(EKirbyCollisionOrder::Ladder, Result))
		{
			// ��ٸ� ������
			LadderTop = Result[0]->GetOwner()->GetActorLocation().Y - Result[0]->GetTransform().GetScale().hY() - 1.f;
			LadderBottom = Result[0]->GetOwner()->GetActorLocation().Y + Result[0]->GetTransform().GetScale().hY() - 1.f;
			StateChange(EKirbyState::LadderDown);
			return;
		}
		if (UContentsHelper::KirbyType != EKirbyType::Eating)
		{
			StateChange(EKirbyState::Crouch);
			return;
		}
		else if (UContentsHelper::KirbyType == EKirbyType::Eating)
		{
			// ��Ű��
			KirbyTypeChange(EKirbyType::Normal);
			StateChange(EKirbyState::Swallow);
			return;
		}
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		BeforeState = EKirbyState::Idle;
		StateChange(EKirbyState::Jump);
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		switch (UContentsHelper::KirbyType)
		{
		case EKirbyType::Normal:
			StateChange(EKirbyState::Inhale);
			return;
		case EKirbyType::Eating:
			StateChange(EKirbyState::Attack);
			return;
		case EKirbyType::Fire:
			StateChange(EKirbyState::Attack);
			return;
		case EKirbyType::Ice:
			StateChange(EKirbyState::Attack);
			return;
		default:
			break;
		}
		//if (true == IsFireKirby || true == IsIceKirby)
		//{
		//	StateChange(EKirbyState::Attack);
		//	return;
		//}
		//StateChange(EKirbyState::Inhale);
		//return;
	}

	//if (true == UEngineInput::IsDown('X') && true == IsEating)
	//{
	//	StateChange(EKirbyState::Attack);
	//	return;
	//}

	// ************* �� �Ʒ��δ� �׽�Ʈ�� *************
	if (true == UEngineInput::IsDown('1'))
	{
		StateChange(EKirbyState::FreeMove);
		return;
	}

	if (true == UEngineInput::IsDown('2'))
	{
		StateChange(EKirbyState::CameraFreeMove);
		return;
	}

	if (true == UEngineInput::IsDown('F'))
	{
		if (UContentsHelper::KirbyType == EKirbyType::Fire) 
		{
			KirbyTypeChange(EKirbyType::Normal);
		}
		else
		{
			KirbyTypeChange(EKirbyType::Fire);
		}
		IdleStart();
		return;
	}

	if (true == UEngineInput::IsDown('I'))
	{
		if (UContentsHelper::KirbyType == EKirbyType::Ice)
		{
			KirbyTypeChange(EKirbyType::Normal);
		}
		else
		{
			KirbyTypeChange(EKirbyType::Ice);
		}
		IdleStart();
		return;
	}
}

// Move : �÷��̾� �̵�(������ ����)
void APlayer::MoveStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Move"));

	if (MoveVector.X == 0.f) 
	{
		MoveVector = { static_cast<float>(DirState) * MovePower, 0.f };
	}
}
void APlayer::Move(float _DeltaTime)
{
	if (DirCheck())	// ���� ��ȯ���� ���
	{
		if (IsPlayerBottomMagentaA())
		{
			// �� ������ ���� �ٲ������ �극��ũ
			StateChange(EKirbyState::Brake);
		}
		else
		{
			PlayerRenderer->ChangeAnimation(GetAnimationName("Move"));
		}
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) && true == UEngineInput::IsPress(VK_RIGHT))
	{
		// ���ÿ� �Էµ��� ���� Idle
		StateChange(EKirbyState::Idle);
		return;
	}

	// �Է¿� ���� �̵� ���
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		MyDir = EActorDir::Left;
		AddMoveVector(FVector::Left * _DeltaTime, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		MyDir = EActorDir::Right;
		AddMoveVector(FVector::Right * _DeltaTime, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);
	}

	// �ȴ� �߿� ��Ÿ �� �޸���
	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		StateChange(EKirbyState::Run);
		return;
	}
	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		StateChange(EKirbyState::Run);
		return;
	}

	if (MyDir == EActorDir::Left && true == UEngineInput::IsFree(VK_LEFT))
	{
		// ����
		AddMoveVector(FVector::Right * _DeltaTime, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);

		// ���� �ӵ� ���ϸ� ���߱�
		if (FinalMoveVector.X > -100.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EKirbyState::Idle);
			return;
		}
	}
	if (MyDir == EActorDir::Right && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// ����
		AddMoveVector(FVector::Left * _DeltaTime, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);

		// ���� �ӵ� ���ϸ� ���߱�
		if (FinalMoveVector.X < 100.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EKirbyState::Idle);
			return;
		}
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		BeforeState = EKirbyState::Move;
		StateChange(EKirbyState::Jump);
		return;
	}
}

// Run : �޸���
void APlayer::RunStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Run"));
	if (MoveVector.X == 0.f)
	{
		MoveVector = { static_cast<float>(DirState) * MovePower, 0.f };
	}
}
void APlayer::Run(float _DeltaTime)
{
	if (DirCheck())
	{
		if (IsPlayerBottomMagentaA())
		{
			// �� ������ ���� �ٲ������ �극��ũ
			StateChange(EKirbyState::Brake);
		}
		else
		{
			PlayerRenderer->ChangeAnimation(GetAnimationName("Run"));
		}
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) && true == UEngineInput::IsPress(VK_RIGHT))
	{
		// ���ÿ� �Էµ��� ���� Idle
		StateChange(EKirbyState::Idle);
		return;
	}

	// �Է¿� ���� �̵� ���
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		MyDir = EActorDir::Left;
		AddMoveVector(FVector::Left * _DeltaTime, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		MyDir = EActorDir::Right;
		AddMoveVector(FVector::Right * _DeltaTime, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);
	}

	if (MyDir == EActorDir::Left && true == UEngineInput::IsFree(VK_LEFT))
	{
		// ����
		AddMoveVector(FVector::Right * _DeltaTime, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);

		// ���� �ӵ� ���ϸ� ���߱�
		if (FinalMoveVector.X > -80.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EKirbyState::Idle);
			return;
		}
	}
	if (MyDir == EActorDir::Right && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// ����
		AddMoveVector(FVector::Left * _DeltaTime, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);

		// ���� �ӵ� ���ϸ� ���߱�
		if (FinalMoveVector.X < 80.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EKirbyState::Idle);
			return;
		}
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		BeforeState = EKirbyState::Run;
		StateChange(EKirbyState::Jump);
		return;
	}
}


// Slide : �����̵�
void APlayer::SlideStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Slide"));

	MoveVector = { static_cast<float>(DirState) * SlideMaxSpeed, 0.f};
}
void APlayer::Slide(float _DeltaTime)
{
	// �����̵��� MoveVector�� üũ
	if (abs(MoveVector.X) < 10.0f)
	{
		MoveVector = FVector::Zero;
		StateChange(EKirbyState::Idle);
		return;
	}
	else
	{
		AddMoveVector({ (-1.0f) * static_cast<float>(DirState) * _DeltaTime, 0.f }, SlideAcc);
		MoveUpdate(_DeltaTime, SlideMaxSpeed, SlideAcc);
		return;
	}
}

// Crouch : ��ũ����
void APlayer::CrouchStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));
}
void APlayer::Crouch(float _DeltaTime)
{
	// ��ũ�� ���¿����� ���� ��ȯ������ ����� ��.
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));

	// ��ũ���� ���¿��� ����Ű ������ �����̵�
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EKirbyState::Slide);
		return;
	}

	if (true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}

// Jump : ����
void APlayer::JumpStart()
{
	DirCheck();
	
	JumpVector = JumpPower;
	PlayerRenderer->ChangeAnimation(GetAnimationName("JumpStart"));
}
void APlayer::Jump(float _DeltaTime)
{
   	if (DirCheck())	// ���߿��� ���� ��ȯ���� ��� ���� �ӵ� �ʱ�ȭ
	{
		MoveVector = FVector::Zero;
		if (UContentsHelper::KirbyType == EKirbyType::Eating)
		{
			PlayerRenderer->ChangeAnimation(GetAnimationName("JumpEnd"));
		}
	}

	if (true == UEngineInput::IsDown('Z') && UContentsHelper::KirbyType != EKirbyType::Eating)
	{
		StateChange(EKirbyState::Fly);
		return;
	}

	// �Է¿� ���� �̵� ���
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * _DeltaTime, MoveAcc);
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * _DeltaTime, MoveAcc);
	}

	MoveUpdate(_DeltaTime, JumpMaxSpeed);

	if (abs(FinalMoveVector.Y) < 40.f && UContentsHelper::KirbyType != EKirbyType::Eating)
	{
		if (FinalMoveVector.Y < 0)
		{
			PlayerRenderer->ChangeAnimation(GetAnimationName("JumpTurn"));
		}
	}

	if (PlayerRenderer->GetCurAnimation()->Name == UEngineString::ToUpper(GetAnimationName("JumpTurn")) && PlayerRenderer->IsCurAnimationEnd() == true && FinalMoveVector.Y > 40.f && false == IsEating)
	{
		PlayerRenderer->ChangeAnimation(GetAnimationName("JumpEnd"));
	}

	if (IsPlayerBottomMagentaA() || IsPlayerBottomYellowA())
	{
		JumpVector = FVector::Zero;
		
		if (UContentsHelper::KirbyType == EKirbyType::Eating)	// Eating ���¿��� ���� ������
		{
			if (PlayerRenderer->GetCurAnimation()->Name == UEngineString::ToUpper(GetAnimationName("JumpStart")) && PlayerRenderer->IsCurAnimationEnd() == true)
			{
				PlayerRenderer->ChangeAnimation(GetAnimationName("JumpEnd"));
			}
			if (PlayerRenderer->GetCurAnimation()->Name == UEngineString::ToUpper(GetAnimationName("JumpEnd")) && PlayerRenderer->IsCurAnimationEnd() == true)
			{
				StateChange(BeforeState);
				return;
			}
		}
		else if(UContentsHelper::KirbyType != EKirbyType::Eating)	// Eating �ƴ� ���¿��� ���� ������
		{
			if (BeforeState == EKirbyState::Idle)
			{
				if (PlayerRenderer->GetCurAnimation()->Name == UEngineString::ToUpper(GetAnimationName("JumpEnd")) && PlayerRenderer->IsCurAnimationEnd() == true)
				{
					PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));
				}
				if (PlayerRenderer->GetCurAnimation()->Name == UEngineString::ToUpper(GetAnimationName("Crouch")) && PlayerRenderer->IsCurAnimationEnd() == true)
				{
					StateChange(BeforeState);
					return;
				}
			}
			else
			{
				StateChange(BeforeState);
				return;
			}
		}
	}
}

// Brake : �̵� ���ߴ� ����
void APlayer::BrakeStart()
{
	DirCheck();
	if (UContentsHelper::KirbyType != EKirbyType::Eating)
	{
		PlayerRenderer->ChangeAnimation(GetAnimationName("Brake"));
	}
	else
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}
void APlayer::Brake(float _DeltaTime)
{
	MoveVector = FVector::Zero;
	if (PlayerRenderer->IsCurAnimationEnd())
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}

// Inhale : ���� �� ����
void APlayer::InhaleStart()
{
	InhaleMaxTime = 0.f;
	InhaleCollision->ActiveOn();

	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("InhaleStart"));
	PlayerSound = UEngineSound::SoundPlay("Inhale.wav");
	PlayerSound.Loop(1);
	InhaleScaleVar = 50.f;
	// Ŀ�� ���⿡ ���� ���� �浹ü ��ġ �ٸ�
	if (DirState == EActorDir::Left)
	{
		InhaleCollision->SetPosition({ -40, -20 });
	}
	else
	{
		InhaleCollision->SetPosition({ 40, -20 });
	}
}
void APlayer::Inhale(float _DeltaTime)
{
	// ���� �浹ü ũ��, ��ġ ������Ʈ
	if (true == UEngineInput::IsPress('X') && InhaleScaleVar <= InhaleMaxScale)
	{
		FTransform ColTrans = InhaleCollision->GetTransform();
		FVector ColScale = ColTrans.GetScale();

		// Scale ���� �̻� ������ InhaleLarge �ִϸ��̼����� ��������� ��.
		if (InhaleScaleVar >= InhaleFirstMax && InhaleScaleVar < InhaleSecondMax)
		{
			PlayerRenderer->ChangeAnimation(GetAnimationName("InhaleSmall"));
		}
		else if (InhaleScaleVar > InhaleSecondMax)
		{
			PlayerRenderer->ChangeAnimation(GetAnimationName("InhaleLarge"));
		}

		InhaleScaleVar += InhaleScaleAdd * _DeltaTime;
		InhaleCollision->SetScale({ InhaleScaleVar, InhaleScaleVar + 20.f });


		FVector ColPos = ColTrans.GetPosition();
		if (DirState == EActorDir::Left)
		{
			InhaleCollision->SetPosition({ ColPos.X - _DeltaTime * InhaleScaleAdd / 2, -20.f });
		}
		else
		{
			InhaleCollision->SetPosition({ ColPos.X + _DeltaTime * InhaleScaleAdd / 2, -20.f });
		}
	}

	// �ð� ����
	if (InhaleScaleVar > InhaleSecondMax)
	{
		InhaleMaxTime += _DeltaTime;
	}

	// ���� �߿� Ŀ��� ������ �浹 Ȯ��
	std::vector<UCollision*> Result1;
	if (true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result1))
	{
		if (Result1[0]->GetOwner()->GetName() == UEngineString::ToUpper("Flamer"))
		{
			UContentsHelper::EatingFireMonster = true;
		}
		// ���� �Ծ������
		InhaleCollision->ActiveOff();
		PlayerSound.Off();
		StateChange(EKirbyState::Eating);
		return;
	}

	std::vector<UCollision*> Result2;
	if (true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::EdibleBullet, Result2))
	{
		// Bullet �Ծ������
		InhaleCollision->ActiveOff();
		PlayerSound.Off();
		StateChange(EKirbyState::Eating);
		return;
	}

	// XŰ ���ų� InhalkeMaxTime�� 2�� �̸��� ���� Idle�� ���ư�
	if (true == UEngineInput::IsFree('X') && InhaleMaxTime < 2.f)
	{
		//UContentsHelper::EatingFireMonster = false;
		PlayerSound.Off();
		InhaleCollision->ActiveOff();
		//IsEating = false;
		StateChange(EKirbyState::Idle);
		return;

	}
	// ���� ���� �ð� �̻� �ϸ� InhaleFail�� ���ư�
	if (InhaleMaxTime >= 2.f)
	{
		InhaleCollision->ActiveOff();
		StateChange(EKirbyState::InhaleFail);
		return;
	}
}

// Eating : �Կ� ���� ���� ����
void APlayer::EatingStart()
{
	DirCheck();
	//IsEating = true;
	KirbyTypeChange(EKirbyType::Eating);
	PlayerRenderer->ChangeAnimation(GetAnimationName("Eating"));
}
void APlayer::Eating(float _DeltaTime)
{
	// Idle�� �Ѱ��ָ� �ִϸ��̼��� �ڵ����� EatingIdle�� ��.
	StateChange(EKirbyState::Idle);
	return;
}

// Swallow : ��Ű��
void APlayer::SwallowStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Swallow"));
}
void APlayer::Swallow(float _DeltaTime)
{
	bool test = UContentsHelper::EatingFireMonster;
	if (true == UContentsHelper::EatingFireMonster)
	{
		//IsFireKirby = true;
		KirbyTypeChange(EKirbyType::Fire);
		UContentsHelper::EatingFireMonster = false;
		StateChange(EKirbyState::Changing);
		return;
	}
	if (PlayerRenderer->IsCurAnimationEnd())
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}

// Attack : ����(�� ��� ���)
const float AttackMaxTime = 0.15f;
void APlayer::AttackStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Attack"));

	switch (UContentsHelper::KirbyType)
	{
	case EKirbyType::Normal:
		break;
	case EKirbyType::Eating:
	{
		PlayerSound = UEngineSound::SoundPlay("Spit_Star.wav");

		AStarBullet* bullet = GetWorld()->SpawnActor<AStarBullet>();
		bullet->SetActorLocation(GetActorLocation() + FVector({ 0, -20 }));
		bullet->SetDir(DirState);
		// bullet�� ���̳� ���Ϳ� ������ �������� �ؾ� ��? �ϴ� Destroy�� �صα�.
		bullet->Destroy(3.f);
		break;
	}
	case EKirbyType::Fire:
	{
		PlayerSound = UEngineSound::SoundPlay("FireAttack.wav");
		PlayerSound.Loop(2);
		TypeAttackTime = AttackMaxTime;

		// ���̾� Ŀ�� ����
		AFireBullet* bullet = GetWorld()->SpawnActor<AFireBullet>();
		bullet->SetDir(DirState);
		bullet->SetActorLocation(GetActorLocation());
		bullet->AddActorLocation(FVector::Down * 20.0f);
		switch (DirState)
		{
		case EActorDir::Left:
			bullet->AddActorLocation(FVector::Left * 80.0f);
			break;
		case EActorDir::Right:
			bullet->AddActorLocation(FVector::Right * 80.0f);
			break;
		default:
			break;
		}
		break;
	}
	case EKirbyType::Ice:
	{
		PlayerSound = UEngineSound::SoundPlay("IceAttack.wav");
		PlayerSound.Loop(2);
		TypeAttackTime = AttackMaxTime;

		// Ice Ŀ�� ����
		AIceBreathBullet* bullet = GetWorld()->SpawnActor<AIceBreathBullet>();
		bullet->SetDir(DirState);
		bullet->SetActorLocation(GetActorLocation());
		//bullet->AddActorLocation(FVector::Down * 20.0f);
		switch (DirState)
		{
		case EActorDir::Left:
			bullet->AddActorLocation(FVector::Left * 80.0f);
			break;
		case EActorDir::Right:
			bullet->AddActorLocation(FVector::Right * 80.0f);
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
}
void APlayer::Attack(float _DeltaTime)
{
	// �� ��� �ٽ� Idle.
	if (EKirbyType::Eating == UContentsHelper::KirbyType && PlayerRenderer->IsCurAnimationEnd())
	{
		KirbyTypeChange(EKirbyType::Normal);
		UContentsHelper::EatingFireMonster = false; 
		StateChange(EKirbyState::Idle);
		return;
	}


	if ((EKirbyType::Fire == UContentsHelper::KirbyType || EKirbyType::Ice == UContentsHelper::KirbyType) && UEngineInput::IsFree('X'))
	{
		PlayerSound.Off();
		StateChange(EKirbyState::Idle);
		return;
	}

	TypeAttackTime -= _DeltaTime;

	if (EKirbyType::Fire == UContentsHelper::KirbyType && UEngineInput::IsPress('X') && 0.0f > TypeAttackTime)
	{
		TypeAttackTime = AttackMaxTime;

		// ���̾� Ŀ�� ����
		AFireBullet* bullet = GetWorld()->SpawnActor<AFireBullet>();
		bullet->SetDir(DirState);
		bullet->SetActorLocation(GetActorLocation());
		bullet->AddActorLocation(FVector::Down * 20.0f);
		switch (DirState)
		{
		case EActorDir::Left:
			bullet->AddActorLocation(FVector::Left * 80.0f);
			break;
		case EActorDir::Right:
			bullet->AddActorLocation(FVector::Right * 80.0f);
			break;
		default:
			break;
		};
	}
	if (EKirbyType::Ice == UContentsHelper::KirbyType && UEngineInput::IsPress('X') && 0.0f > TypeAttackTime)
	{
		TypeAttackTime = AttackMaxTime;
		// Ice Ŀ�� ����
		AIceBreathBullet* bullet = GetWorld()->SpawnActor<AIceBreathBullet>();
		bullet->SetDir(DirState);
		bullet->SetActorLocation(GetActorLocation());
		//bullet->AddActorLocation(FVector::Down * 20.0f);
		switch (DirState)
		{
		case EActorDir::Left:
			bullet->AddActorLocation(FVector::Left * 80.0f);
			break;
		case EActorDir::Right:
			bullet->AddActorLocation(FVector::Right * 80.0f);
			break;
		default:
			break;
		}
	}
}

void APlayer::FlyStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("FlyStart"));
}
void APlayer::Fly(float _DeltaTime)
{
	DirCheck();
	WoodBlockCheck();

	if (true == IsPlayerFrontMagentaA())
	{
		switch (DirState)
		{
		case EActorDir::Left:
			AddActorLocation(FVector::Right * 5.f);
			break;
		case EActorDir::Right:
			AddActorLocation(FVector::Left * 5.f);
			break;
		default:
			break;
		}
		return;
	}

	if (true == IsPlayerTopMagentaA())
	{		
		FVector AddV = FVector::Down * FlySpeed * _DeltaTime;
		AddActorLocation(AddV);
		CameraMove(AddV);
		return;
	}

	if (CurAnimationName == "FlyStart" && PlayerRenderer->IsCurAnimationEnd() == true)
	{
		PlayerRenderer->ChangeAnimation(GetAnimationName("Flying"));
	}

	if (true == UEngineInput::IsPress('X'))
	{
		StateChange(EKirbyState::Exhale);
		return;
	}

	if (true == UEngineInput::IsPress(VK_UP) || true == UEngineInput::IsPress('Z'))
	{
		FVector AddV = FVector::Up * (FlySpeed + 20.f) * _DeltaTime;
		AddActorLocation(AddV);
		CameraMove(AddV);
	}
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		FVector AddV = FVector::Left * FlySpeed * _DeltaTime;
		AddActorLocation(AddV);
		CameraMove(AddV);
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		FVector AddV = FVector::Right * FlySpeed * _DeltaTime;
		AddActorLocation(AddV);
		CameraMove(AddV);
	}
	//if (true == UEngineInput::IsPress(VK_DOWN))
	//{
	//	// �������� �� �߷¿� ���ؼ��� �ϵ��� ��� ����
	//	//AddActorLocation(FVector::Down * FlySpeed * _DeltaTime);
	//}
	if (true == UEngineInput::IsFree(VK_UP) && true == UEngineInput::IsFree('Z'))
	{
		if (IsPlayerBottomMagentaA() || IsPlayerBottomYellowA())
		{
			UpMoving(_DeltaTime, Color8Bit::MagentaA);
		} 
		else
		{
			FVector AddV = FVector::Down * FlySpeed * _DeltaTime;
			AddActorLocation(AddV);
			CameraMove(AddV);
		}
	}

}

void APlayer::LadderUpStart()
{
	PlayerRenderer->ChangeAnimation(GetAnimationName("LadderUp"));
}
void APlayer::LadderUp(float _DeltaTime)
{
	if (true == UEngineInput::IsFree(VK_UP) && true == UEngineInput::IsPress(VK_DOWN))
	{
		StateChange(EKirbyState::LadderDown);
		return;
	}
	std::vector<UCollision*> Result;
	if (false == BodyCollision->CollisionCheck(EKirbyCollisionOrder::Ladder, Result))
	{
		//float CurX = GetActorLocation().X;
		//SetActorLocation({ CurX, LadderTop });
		StateChange(EKirbyState::Idle);
		return;
	}
	if (true == UEngineInput::IsPress(VK_UP))
	{
		// ���� �ø��� �ڵ�
		FVector LPos = LadderUpSpeed * _DeltaTime;
		AddActorLocation(LPos);
		CameraMove(LPos);
	}
}

void APlayer::LadderDownStart()
{
	PlayerRenderer->ChangeAnimation(GetAnimationName("LadderDown"));
}
void APlayer::LadderDown(float _DeltaTime)
{
	if (true == UEngineInput::IsFree(VK_DOWN) && true == UEngineInput::IsPress(VK_UP))
	{
		StateChange(EKirbyState::LadderUp);
		return;
	}

	std::vector<UCollision*> Result;
	if (false == BottomCollision->CollisionCheck(EKirbyCollisionOrder::Ladder, Result))
	{
		StateChange(EKirbyState::Idle);
		return;
	}

	if (true == UEngineInput::IsPress(VK_DOWN))
	{
		FVector LPos = LadderDownSpeed * _DeltaTime;
		// ���� ������ �ڵ�
		AddActorLocation(LPos);
		CameraMove(LPos);
	}
}

void APlayer::ExhaleStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Exhale"));
	PlayerSound = UEngineSound::SoundPlay("Exhale.wav");
	MoveVector = FVector::Zero;
	JumpVector = FVector::Zero;
	GravityVector = FVector::Zero;
}
void APlayer::Exhale(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);
	if (PlayerRenderer->IsCurAnimationEnd() == true)
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}

void APlayer::DamagedStart()
{
	DirCheck();
	AddDamageHp(DamagePower);	// ������ ����, Ŀ�� �״� �� ���� �����̱� ��!!
	PlayerRenderer->ChangeAnimation(GetAnimationName("Damaged"));
	PlayerSound = UEngineSound::SoundPlay("Damaged.wav");
	BodyCollision->ActiveOff();
	FrontCollision->ActiveOn();				// �����浹 �����ϰ� �ϱ����� �ݸ��� �ܻ�?�� ����..
	AlphaTime = 1.f;

	MoveVector = FVector::Zero;
	switch (DirState)
	{
	case EActorDir::Left:
		MoveVector += FVector::Right * 100.f;
		break;
	case EActorDir::Right:
		MoveVector += FVector::Left * 100.f;
		break;
	default:
		break;
	}
}
void APlayer::Damaged(float _DeltaTime)
{
	AlphaTime -= _DeltaTime;

	if (AlphaTime > 0.f)
	{
		PlayerRenderer->SetAlpha(AlphaVar);
		AlphaVar = 1.f - AlphaVar;
	}

	// ����
	switch (DirState)
	{
	case EActorDir::Left:
		AddMoveVector(FVector::Left * _DeltaTime, SmallMoveAcc);
		break;
	case EActorDir::Right:
		AddMoveVector(FVector::Right * _DeltaTime, SmallMoveAcc);
		break;
	default:
		break;
	}
	MoveUpdate(_DeltaTime);

	// ���� �ӵ� ���ϸ� ���߱�
	if (true == PlayerRenderer->IsCurAnimationEnd() && abs(FinalMoveVector.X) < 100.0f && AlphaTime <= 0.f)
	{
		PlayerRenderer->SetAlpha(1.0f);
		FrontCollision->ActiveOff();
		BodyCollision->ActiveOn();
		MoveVector = FVector::Zero;
		StateChange(EKirbyState::Idle);
		return;
	}
}

void APlayer::InhaleFailStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("InhaleFail"));
}
void APlayer::InhaleFail(float _DeltaTime)
{
	if (true == PlayerRenderer->IsCurAnimationEnd())
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}

void APlayer::ChangingStart()
{
	ChangingTime = 0.f;
	// �����ϴ� �̹��� �߰��ؾ� ��
	PlayerRenderer->AnimationReset();
	PlayerRenderer->SetImage("HappyKirby.png"); // �ӽ� ���ҽ�
	PlayerSound = UEngineSound::SoundPlay("Changing.wav");

	GetWorld()->SetOtherTimeScale(EKirbyRenderOrder::Player, 0.f);
	GetWorld()->SetTimeScale(EKirbyRenderOrder::UI, 1.f);	
}
void APlayer::Changing(float _DeltaTime)
{
	ChangingTime += _DeltaTime;
	if (ChangingTime > 1.5f)
	{
		GetWorld()->SetAllTimeScale(1.0f);
		StateChange(EKirbyState::Idle);
		return;
	}
}

// FreeMove : ������ ĳ���� ���� �̵�
void APlayer::FreeMove(float _DeltaTime)
{
	FVector MovePos = FVector::Zero;

	if (UEngineInput::IsPress(VK_LEFT))
	{
		MovePos += FVector::Left * _DeltaTime * FreeMoveSpeed;
	}

	if (UEngineInput::IsPress(VK_RIGHT))
	{
		MovePos += FVector::Right * _DeltaTime * FreeMoveSpeed;
	}

	if (UEngineInput::IsPress(VK_UP))
	{
		MovePos += FVector::Up * _DeltaTime * FreeMoveSpeed;
	}

	if (UEngineInput::IsPress(VK_DOWN))
	{
		MovePos += FVector::Down * _DeltaTime * FreeMoveSpeed;
	}

	AddActorLocation(MovePos);
	GetWorld()->AddCameraPos(MovePos);

	if (UEngineInput::IsDown('1'))
	{
		StateChange(EKirbyState::Idle);
	}
}

// FreeMove : ������ ī�޶� ���� �̵�
void APlayer::CameraFreeMove(float _DeltaTime)
{
	if (UEngineInput::IsPress(VK_LEFT))
	{
		GetWorld()->AddCameraPos(FVector::Left * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsPress(VK_RIGHT))
	{
		GetWorld()->AddCameraPos(FVector::Right * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsPress(VK_UP))
	{
		GetWorld()->AddCameraPos(FVector::Up * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsPress(VK_DOWN))
	{
		GetWorld()->AddCameraPos(FVector::Down * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsDown('2'))
	{
		StateChange(EKirbyState::Idle);
	}
}

std::string APlayer::GetAnimationName(std::string _Name)
{
	std::string DirName = "";
	std::string resName = "";

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

	switch (UContentsHelper::KirbyType)
	{
	case EKirbyType::Normal:
		resName = _Name;
		break;
	case EKirbyType::Eating:
		resName = std::string("Eating") + _Name;
		break;
	case EKirbyType::Fire:
		resName = std::string("Fire") + _Name;
		break;
	case EKirbyType::Ice:
		resName = std::string("Ice") + _Name;
		break;
	default:
		resName = _Name;
		break;
	}

	if (_Name == "LadderUp" || _Name == "LadderDown")
	{
		return resName;
	}

	return resName + DirName;

}

// �����̰� �ִ� ���� �˷���
bool APlayer::DirCheck()
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
		IsChanged = true;
		if (UContentsHelper::KirbyType == EKirbyType::Eating && CurAnimationName == "JumpTurn")
		{
			return IsChanged;
		}
		std::string Name = GetAnimationName(CurAnimationName);
		PlayerRenderer->ChangeAnimation(Name);
	}

	return IsChanged;
}

void APlayer::UpMoving(float _DeltaTime, Color8Bit _Color)
{
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color == _Color)
		{
			AddActorLocation(FVector::Up);
			if (GetWorld()->GetName() == "BOSSLEVEL")
			{
				GetWorld()->AddCameraPos(FVector::Up);
			}
		}
		else 
		{
			break;
		}
	}
}

// MoveUpdate : �ۿ��ϴ� �� ����ؼ� �̵�
void APlayer::MoveUpdate(float _DeltaTime, float MaxSpeed/* = 0.0f*/, FVector Acc /*= FVector::Zero*/)
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

void APlayer::AddMoveVector(const FVector& _DirDelta, FVector Acc)
{
	MoveVector += _DirDelta * Acc;
}

void APlayer::CalMoveVector(float _DeltaTime, float MaxSpeed)
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

void APlayer::CalGravityVector(float _DeltaTime)
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

void APlayer::CalFinalMoveVector(float _DeltaTime)
{
	FinalMoveVector = FVector::Zero;
	FinalMoveVector += MoveVector;
	FinalMoveVector += GravityVector;
	FinalMoveVector += JumpVector;
}

// ���� ���� ����� ������ �̵���Ű�� �Լ�
void APlayer::FinalMove(float _DeltaTime) 
{
	MapSize = UContentsHelper::ColMapImage->GetScale();

	FVector MovePos = FinalMoveVector * _DeltaTime;					// �÷��̾� �̵��� (�ȱ��� Move�� �ƴ�)

	FVector PrevPlayerPos = GetActorLocation();
	FVector NextPlayerPos = PrevPlayerPos + MovePos;

	Color8Bit TopColor = UContentsHelper::ColMapImage->GetColor(NextPlayerPos.iX(), NextPlayerPos.iY() - 20, Color8Bit::MagentaA);
	if (TopColor == Color8Bit::MagentaA || TopColor == Color8Bit::YellowA)
	{
		if (UContentsHelper::KirbyType != EKirbyType::Eating /*&& false == IsFireKirby && false == IsIceKirby*/)
		{
			MovePos.Y = 0.f;
			PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));	// �Ӹ����� �� ��εǴ°� TestCode
			return;
		}
	}

	// �÷��̾� �̵�
	AddActorLocation(MovePos);
	CameraMove(MovePos);
	WoodBlockCheck();
}

bool APlayer::IsPlayerFrontMagentaA()
{
	Color8Bit ColorL = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX() - 20, GetActorLocation().iY() - 20, Color8Bit::MagentaA);
	Color8Bit ColorR = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX() + 20, GetActorLocation().iY() - 20, Color8Bit::MagentaA);
	if (ColorL == Color8Bit::MagentaA || ColorR == Color8Bit::MagentaA)
	{
		return true;
	}
	return false;
}

bool APlayer::IsPlayerBottomMagentaA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA)
	{
		return true;
	}
	return false;
}

bool APlayer::IsPlayerTopMagentaA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() - 40, Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA)
	{
		return true;
	}
	return false;
}

bool APlayer::IsPlayerBottomYellowA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::YellowA)
	{
		return true;
	}
	return false;
}

bool APlayer::IsPlayerDoor()
{
	FVector ActorLoc = GetActorLocation();
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(ActorLoc.iX(), ActorLoc.iY() - 10, Color8Bit::MagentaA);
	if (Color == Color8Bit::GreenA)
	{
		return true;
	}
	return false;
}

void APlayer::CameraMove(FVector MovePos)
{
	{
		// ī�޶� �¿��̵�
		FVector CameraPosLeft = GetWorld()->GetCameraPos();				// ī�޶� ���� ��ǥ
		FVector NextCameraPosLeft = CameraPosLeft + MovePos;			// �÷��̾� �̵� �� ���� ī�޶� �� 
		FVector NextCameraPosRight = WinScale + NextCameraPosLeft;		// �̵� ���� ī�޶� ������ ��
		FVector NextPlayerPos = GetActorLocation();						// �÷��̾� ��ġ
		FVector XCam = { MovePos.X, 0.f };
		if (
			NextPlayerPos.X >= NextCameraPosLeft.X + WinScale.hX() &&	// �÷��̾� ��ġ�� �� ���� ������ ���� �̻��� ������ ī�޶� �̵��ϵ���
			NextPlayerPos.X <= MapSize.X - WinScale.hX() &&				// �÷��̾� ��ġ�� �� ������ ������ ������ �������� ī�޶� ���������
			NextCameraPosLeft.X >= 0 &&									// ī�޶� �� ������ �� ��������
			NextCameraPosRight.X <= MapSize.X
			)
		{
			GetWorld()->AddCameraPos(XCam);
		}
	}

	{
		// ī�޶� �����̵�
		FVector CameraPos = GetWorld()->GetCameraPos();
		FVector NextPlayerPos = GetActorLocation();
		FVector YCam = { 0.f, MovePos.Y };
		FVector NextCameraPos = CameraPos + YCam;
		// BossLevel������ �÷��̾ WinScale ���� �̻��� �� ī�޶� ���󰡾� ��.
		if (
			NextPlayerPos.Y >= WinScale.hY() &&							// �÷��̾� ��ġ�� �� ���� ������ ���� �̻��� ������ ī�޶� �̵��ϵ���
			NextPlayerPos.Y <= MapSize.Y - WinScale.hY() &&				// �÷��̾� ��ġ�� �� �Ʒ��� ������ ������ �������� ī�޶� ���������
			NextCameraPos.Y >= 0 &&										// ī�޶� �� ������ �� ��������
			NextCameraPos.Y + WinScale.Y <= MapSize.Y
			)
		{
			GetWorld()->AddCameraPos(YCam);
		}
	}
}

void APlayer::WoodBlockCheck()
{
	std::vector<UCollision*> Result;
	if (true == RealBottomCollision->CollisionCheck(EKirbyCollisionOrder::Block, Result))
	{
		while (true)
		{
			std::vector<UCollision*> Result2;
			if (true == RealBottomCollision->CollisionCheck(EKirbyCollisionOrder::Block, Result2))
			{
				GravityVector = FVector::Zero;
				JumpVector = FVector::Zero;
				AddActorLocation(FVector::Up);
				GetWorld()->AddCameraPos(FVector::Up);
			}
			else
			{
				break;
			}
		}
	}
}