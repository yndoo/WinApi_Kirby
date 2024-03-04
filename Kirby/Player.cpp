#include "Player.h"
#include<EnginePlatform/EngineInput.h>
#include "ContentsHelper.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineDebug.h>
#include "StarBullet.h"
#include "FireBullet.h"

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
	IsEating = Kirby->IsEating;
	IsFireKirby = Kirby->IsFireKirby;
}

void APlayer::BeginPlay() {
	AActor::BeginPlay();

 	if (nullptr != Kirby)
	{
		// Ŀ�� �̹� �ִ� ���
		KirbyCopy();
		Kirby->Destroy();

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
		SetActorLocation({ 100, 100 });
	}

	Kirby = this;

	PlayerRenderer = CreateImageRenderer(EKirbyRenderOrder::Player);
	PlayerRenderer->SetImage("Kirby.png");
	PlayerRenderer->SetTransform({ {0,0}, {300, 300} });
	
	PlayerRenderer->SetTransColor(Color8Bit::Magenta);

	AutoCreateAnimation("Move", { 0,1,1,2,3,4,5,6,6,7,8,9 }, 0.07f, true);
	AutoCreateAnimation("Idle", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2 }, 0.09f, true);
	AutoCreateAnimation("Crouch", 1, 1, 0.3f, false);
	AutoCreateAnimation("Slide", 0, 0, 0.3f, true);
	AutoCreateAnimation("Run", 0, 7, 0.05f, true);
	AutoCreateAnimation("Break", 0, 0, 0.2f, false);
	AutoCreateAnimation("Swallow", "Swallow", 0, 4, 0.1f, false);
	AutoCreateAnimation("InhaleStart", "Inhale", 4, 4, 0.1f, false);
	AutoCreateAnimation("InhaleSmall", "Inhale", 5, 6, 0.1f, true);
	AutoCreateAnimation("InhaleLarge", "Inhale", 7, 8, 0.1f, true);
	AutoCreateAnimation("InhaleFail", "Inhale", 9, 12, 0.1f, false);
	AutoCreateAnimation("JumpTurn", "Jump", 1, 8, 0.03f, false);
	AutoCreateAnimation("JumpStart", "Jump", 0, 0, 0.1f, false);

	AutoCreateAnimation("EatingAttack", 0, 4, 0.1f, false);
	AutoCreateAnimation("EatingEating", "Eating", 2, 6, 0.1f, false);
	AutoCreateAnimation("EatingIdle", "Eating", 6, 6, 0.1f, false);
	AutoCreateAnimation("EatingRun", "EatingMove", 0, 14, 0.05f, true);
	AutoCreateAnimation("EatingMove", "EatingMove", 0, 14, 0.07f, true);
	AutoCreateAnimation("EatingJumpStart", "EatingJump", {0,1,2,3,4,5,6}, 0.08f, false);
	AutoCreateAnimation("EatingJumpEnd", "EatingJump", 7, 8, 0.1f, false);

	AutoCreateAnimation("FireIdle", {0,1,2,3,0,1,2,3,4,5,6,7}, 0.09f, true);
	AutoCreateAnimation("FireMove", 0, 19, 0.04f, true);
	AutoCreateAnimation("FireSlide", 0, 4, 0.06f, false);
	AutoCreateAnimation("FireBreak", 0, 1, 0.1f, false);
	AutoCreateAnimation("FireRun", 0, 7, 0.05f, true);
	AutoCreateAnimation("FireCrouch", 0, 7, 0.15f, true);
	AutoCreateAnimation("FireAttack", 0, 3, 0.05f, true);
	AutoCreateAnimation("FireJumpTurn", "FireJump", 2, 8, 0.03f, false);
	AutoCreateAnimation("FireJumpStart", "FireJump", 0, 1, 0.1f, false);


	PlayerRenderer->ChangeAnimation("Idle_Right");


	BodyCollision = CreateCollision(EKirbyCollisionOrder::Player);
	BodyCollision->SetScale({ 45, 0 });
	BodyCollision->SetPosition({ 0, -20 });
	BodyCollision->SetColType(ECollisionType::CirCle);

	InhaleCollision = CreateCollision(EKirbyCollisionOrder::InhaleCol);
	InhaleCollision->SetScale({ 100, 0 });		// ���� �浹ü ũ��� ���� �Է� �ð��� ���� �޶�����.
	InhaleCollision->SetPosition({ 0, -20 });	// ���� �浹ü ��ġ�� ���� �ø��� �ٲ��� �Ѵ�.
	InhaleCollision->SetColType(ECollisionType::Rect);
	InhaleCollision->ActiveOff();

	StateChange(EKirbyState::Idle);
}

void APlayer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

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
		case EKirbyState::Break:
			BreakStart();
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
	case EKirbyState::Break:
		// �극��ũ
		Break(_DeltaTime);
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

	if (true == UEngineInput::IsPress(VK_DOWN))
	{
		if (false == IsEating)
		{
			StateChange(EKirbyState::Crouch);
			return;
		}
		else
		{
			// ��Ű��
			IsEating = false;
			StateChange(EKirbyState::Swallow);
			return;
		}
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		BeforeJumpState = EKirbyState::Idle;
		StateChange(EKirbyState::Jump);
		return;
	}

	if (true == UEngineInput::IsPress('X') && false == IsEating)
	{
		if (true == IsFireKirby)
		{
			StateChange(EKirbyState::Attack);
			return;
		}
		StateChange(EKirbyState::Inhale);
		return;
	}

	if (true == UEngineInput::IsDown('X') && true == IsEating)
	{
		StateChange(EKirbyState::Attack);
		return;
	}

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
		IsFireKirby = !IsFireKirby;
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
			StateChange(EKirbyState::Break);
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
		BeforeJumpState = EKirbyState::Move;
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
			StateChange(EKirbyState::Break);
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
		BeforeJumpState = EKirbyState::Run;
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
	}

	// ���� �Է¿� ���� �̵� ���
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * _DeltaTime, MoveAcc);
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * _DeltaTime, MoveAcc);
	}

	MoveUpdate(_DeltaTime, JumpMaxSpeed);

	if (abs(FinalMoveVector.Y) < 50.f && false == IsEating)
	{
		PlayerRenderer->ChangeAnimation(GetAnimationName("JumpTurn"));
	}

	if (IsPlayerBottomMagentaA() || IsPlayerBottomYellow())
	{
		JumpVector = FVector::Zero;
		if (true == IsEating && BeforeJumpState == EKirbyState::Idle)
		{
			if (PlayerRenderer->GetCurAnimation()->Name == UEngineString::ToUpper(GetAnimationName("JumpEnd")) && PlayerRenderer->IsCurAnimationEnd() == true)
			{
				StateChange(BeforeJumpState);
				return;
			}
			PlayerRenderer->ChangeAnimation(GetAnimationName("JumpEnd"));
		}
		else
		{
			StateChange(BeforeJumpState);
			return;
		}
	}
}

// Break : �̵� ���ߴ� ����
void APlayer::BreakStart()
{
	DirCheck();
	if (false == IsEating)
	{
		PlayerRenderer->ChangeAnimation(GetAnimationName("Break"));
	}
	else
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}
void APlayer::Break(float _DeltaTime)
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
	InhaleCollision->ActiveOn();

	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("InhaleStart"));
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

		// Scale ���� �̻� ������ InhaleLarge �ִϸ��̼����� ��������� ��. (���� �� ��)

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

	// ���� �߿� Ŀ��� ������ �浹 Ȯ��
	std::vector<UCollision*> Result;
	if (true == BodyCollision->CollisionCheck(EKirbyCollisionOrder::Monster, Result))
	{
		// ���� �Ծ������

		AActor* temp = Result[0]->GetOwner();
		int a = 0;

		//if(Result[0]->GetOwner()->CopyAbilityType)
		InhaleCollision->ActiveOff();
		StateChange(EKirbyState::Eating);
		return;
	}

	// �� �ʵ��� ���Կ� �� �� ������ ���� �׳� ����.. ���߿� ����.
	// ���� ����
	if (true == UEngineInput::IsFree('X'))
	{
		InhaleCollision->ActiveOff();
		IsEating = false;
		StateChange(EKirbyState::Idle);
		return;
	}
}

// Eating : �Կ� ���� ���� ����
void APlayer::EatingStart()
{
	DirCheck();
	IsEating = true;
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
	if (PlayerRenderer->IsCurAnimationEnd())
	{
		StateChange(EKirbyState::Idle);
		return;
	}
}

// Attack : ����(�� ��� ���)
const float FireMaxTime = 0.15f;
void APlayer::AttackStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Attack"));

	if (true == IsEating)
	{
		AStarBullet* bullet = GetWorld()->SpawnActor<AStarBullet>();
		bullet->SetActorLocation(GetActorLocation() + FVector({ 0, -20 }));
		bullet->SetDir(DirState);
		// bullet�� ���̳� ���Ϳ� ������ �������� �ؾ� ��? �ϴ� Destroy�� �صα�.
		bullet->Destroy(3.f);
	}
	else if (true == IsFireKirby)
	{
		FireTime = FireMaxTime;

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
	}
}
void APlayer::Attack(float _DeltaTime)
{
	// �� ��� �ٽ� Idle.
	if (true == IsEating && PlayerRenderer->IsCurAnimationEnd())
	{
		IsEating = false;
		StateChange(EKirbyState::Idle);
		return;
	}

	FireTime -= _DeltaTime;

	if (true == IsFireKirby && UEngineInput::IsFree('X'))
	{
		StateChange(EKirbyState::Idle);
		return;
	}

	if (true == IsFireKirby && UEngineInput::IsPress('X') && 0.0f > FireTime)
	{
		FireTime = FireMaxTime;

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

	if (IsEating == true) 
	{
		return std::string("Eating") + _Name + DirName;
	}
	else if (IsFireKirby == true)
	{
		return std::string("Fire") + _Name + DirName;
	}

	return _Name + DirName;

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
		std::string Name = GetAnimationName(CurAnimationName);
		PlayerRenderer->ChangeAnimation(Name);
		IsChanged = true;
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
	if (IsPlayerBottomYellow())
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

	if (IsPlayerBottomYellow())
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

	// �÷��̾� �̵�
	AddActorLocation(MovePos);

	// ī�޶� �̵�
	FVector CameraPosLeft = GetWorld()->GetCameraPos();				// ī�޶� ���� ��ǥ
	FVector NextCameraPosLeft = CameraPosLeft + MovePos;			// �÷��̾� �̵� �� ���� ī�޶� �� 
	FVector NextCameraPosRight = WinScale + NextCameraPosLeft;		// �̵� ���� ī�޶� ������ ��
	FVector NextPlayerPos = GetActorLocation();						// �÷��̾� ��ġ

	if (
		NextPlayerPos.X >= NextCameraPosLeft.X + WinScale.hX() &&	// �÷��̾� ��ġ�� �� ���� ������ ���� �̻��� ������ ī�޶� �̵��ϵ���
		NextPlayerPos.X <= MapSize.X - WinScale.hX() &&				// �÷��̾� ��ġ�� �� ������ ������ ������ �������� ī�޶� ���������
		NextCameraPosLeft.X >= 0 &&									// ī�޶� �� ������ �� ��������
		NextCameraPosRight.X <= MapSize.X
		)
	{
		GetWorld()->AddCameraPos(MoveVector * _DeltaTime);
	}
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

bool APlayer::IsPlayerBottomYellow()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::YellowA)
	{
		return true;
	}
	return false;
}

// ���� ���� �Լ�.. (��� X)
bool APlayer::IsPlayerOnYellow()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	Color8Bit ColorLeft = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX() - 15, GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	Color8Bit ColorRight = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX() + 15, GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit::YellowA || ColorLeft == Color8Bit::YellowA || ColorRight == Color8Bit::YellowA)
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