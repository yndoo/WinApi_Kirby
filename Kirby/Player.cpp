#include "Player.h"
#include<EnginePlatform/EngineInput.h>
#include "ContentsHelper.h"
#include <EngineCore/EngineCore.h>

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

void APlayer::BeginPlay() {
	AActor::BeginPlay();


 	if (nullptr != Kirby)
	{
		// Ŀ�� �̹� �ִ� ���
		// ���� ����� ���� Ŀ�� ����Ǿ��ִ� BeforePos�� �޾ƾ� ��.
		// �ٵ� RestAreaLevel������ BeforePos�� ���� �� ��...
		BeforePos = Kirby->BeforePos;
		Kirby->Destroy();
		SetActorLocation(BeforePos);
	}
	else
	{
		SetActorLocation({ 100, 100 });
	}
	std::string name = GetWorld()->GetName();
	if (GetWorld()->GetName() == "RESTAREALEVEL")
	{
		SetActorLocation({ 100, 100 });
	}
	Kirby = this;

	PlayerRenderer = CreateImageRenderer(0);
	PlayerRenderer->SetImage("Kirby.png");
	PlayerRenderer->SetTransform({ {0,0}, {300, 300} });
	
	PlayerRenderer->SetTransColor(Color8Bit::Magenta);

	AutoCreateAnimation("Move", { 0,1,1,2,3,4,5,6,6,7,8,9 }, 0.07f, true);
	AutoCreateAnimation("Idle", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2 }, 0.09f, true);
	AutoCreateAnimation("Crouch", 1, 1, 0.3f, true);
	AutoCreateAnimation("Slide", 0, 0, 0.3f, true);
	AutoCreateAnimation("Run", 0, 7, 0.05f, true);
	AutoCreateAnimation("Break", 0, 0, 0.2f, false);
	AutoCreateAnimation("JumpTurn", "Jump", 1, 8, 0.03f, false);
	AutoCreateAnimation("JumpStart", "Jump", 0, 0, 0.1f, false);
	AutoCreateAnimation("InhaleStart", "Inhale", 4, 4, 0.1f, false);
	AutoCreateAnimation("InhaleSmall", "Inhale", 5, 6, 0.1f, true);
	AutoCreateAnimation("InhaleLarge", "Inhale", 7, 8, 0.1f, true);
	AutoCreateAnimation("InhaleFail", "Inhale", 9, 12, 0.1f, false);

	// ���� �̹����� ���� �ִϸ��̼��� �����ϱ� ������ ���� ����
	//PlayerRenderer->CreateAnimation("JumpTurn_Right", "Jump_Right.png", 1, 8, 0.03f, false);	// ���� ȸ��
	//PlayerRenderer->CreateAnimation("JumpTurn_Left", "Jump_Left.png", 1, 8, 0.03f, false);
	//PlayerRenderer->CreateAnimation("JumpStart_Right", "Jump_Right.png", 0, 0, 0.1f, false);	// ���� ����
	//PlayerRenderer->CreateAnimation("JumpStart_Left", "Jump_Left.png", 0, 0, 0.1f, false);

	PlayerRenderer->ChangeAnimation("Idle_Right");


	BodyCollision = CreateCollision(KirbyCollisionOrder::Player);
	BodyCollision->SetScale({ 50, 0 });
	BodyCollision->SetPosition({ 0, -20 });
	BodyCollision->SetColType(ECollisionType::CirCle);

	InhaleCollision = CreateCollision(KirbyCollisionOrder::PlayerBullet);
	InhaleCollision->SetScale({ 100, 0 });		// ���� �浹ü ũ��� ���� �Է� �ð��� ���� �޶�����.
	InhaleCollision->SetPosition({ 0, -20 });	// ���� �浹ü ��ġ�� ���� �ø��� �ٲ��� �Ѵ�.
	InhaleCollision->SetColType(ECollisionType::Rect);

	StateChange(EPlayState::Idle);
}

void APlayer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	FVector PlayerPos = GetActorLocation();
	int  a = 0;
}

void APlayer::StateChange(EPlayState _State) 
{
	if (State != _State)
	{
		switch (_State)
		{
		case EPlayState::Idle:
			IdleStart();
			break;
		case EPlayState::Move:
			MoveStart();
			break;
		case EPlayState::Run:
			RunStart();
			break;
		case EPlayState::Slide:
			SlideStart();
			break;
		case EPlayState::Crouch:
			CrouchStart();
			break;
		case EPlayState::Jump:
			JumpStart();
			break;
		case EPlayState::Break:
			BreakStart();
			break;
		case EPlayState::Inhale:
			InhaleStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void APlayer::StateUpdate(float _DeltaTime) {
	switch (State) {
	case EPlayState::Idle:
		// ������
		Idle(_DeltaTime);
		break;
	case EPlayState::Move:
		// �̵�
		Move(_DeltaTime);
		break;
	case EPlayState::Crouch:
		// ��ũ����
		Crouch(_DeltaTime);
		break;
	case EPlayState::Slide:
		// �����̵�
		Slide(_DeltaTime);
		break;
	case EPlayState::Run:
		// �޸���
		Run(_DeltaTime);
		break;
	case EPlayState::Jump:
		// ����
		Jump(_DeltaTime);
		break;
	case EPlayState::Break:
		// �극��ũ
		Break(_DeltaTime);
		break;
	case EPlayState::Inhale:
		// ����
		Inhale(_DeltaTime);
		break;
	case EPlayState::FreeMove:
		// ���� �̵�
		FreeMove(_DeltaTime);
		break;
	case EPlayState::CameraFreeMove:
		// ī�޶� ���� �̵�
		CameraFreeMove(_DeltaTime);
		break;
	default:
		break;
	}
}

void APlayer::IdleStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Idle"));
}

void APlayer::MoveStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Move"));

	if (MoveVector.X == 0.f) 
	{
		MoveVector = { static_cast<float>(DirState) * 100, 0.f };
	}
}

void APlayer::RunStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Run"));
	if (MoveVector.X == 0.f)
	{
		MoveVector = { static_cast<float>(DirState) * 100, 0.f };
	}
}

void APlayer::SlideStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Slide"));

	MoveVector = { static_cast<float>(DirState) * SlideMaxSpeed, 0.f};
}

void APlayer::CrouchStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));
}

void APlayer::JumpStart()
{
	DirCheck();
	
	JumpVector = JumpPower;
	PlayerRenderer->ChangeAnimation(GetAnimationName("JumpStart"));
}

void APlayer::BreakStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Break"));
}

void APlayer::InhaleStart()
{
	InhaleCollision->ActiveOn();

	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("InhaleStart")); // �ӽ� �ִϸ��̼�
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

// Idle : ������ �ִ� ����
void  APlayer::Idle(float _DeltaTime) {
	MoveVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDoubleClick(VK_RIGHT, 0.3f))
	{
		StateChange(EPlayState::Run);
		return;
	}
	if (true == UEngineInput::IsDoubleClick(VK_LEFT, 0.3f))
	{
		StateChange(EPlayState::Run);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
	{
		// �׳� Move
		StateChange(EPlayState::Move);
		return;
	}

	if (true == UEngineInput::IsPress(VK_DOWN))
	{
		StateChange(EPlayState::Crouch);
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		BeforeJumpState = EPlayState::Idle;
		StateChange(EPlayState::Jump);
		return;
	}

	if (true == UEngineInput::IsPress('X'))
	{
		StateChange(EPlayState::Inhale);
		return;
	}

	if (true == UEngineInput::IsDown('1'))
	{
		StateChange(EPlayState::FreeMove);
		return;
	}

	if (true == UEngineInput::IsDown('2'))
	{
		StateChange(EPlayState::CameraFreeMove);
		return;
	}
}

// Move : �÷��̾� �̵�(������ ����)
void APlayer::Move(float _DeltaTime)
{
	if (DirCheck())	// ���� ��ȯ���� ���
	{
		if (IsPlayerBottomMagentaA())
		{
			// �� ������ ���� �ٲ������ �극��ũ
			MoveVector = FVector::Zero;
			StateChange(EPlayState::Break);
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
		StateChange(EPlayState::Idle);
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
		StateChange(EPlayState::Run);
		return;
	}
	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		StateChange(EPlayState::Run);
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
			StateChange(EPlayState::Idle);
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
			StateChange(EPlayState::Idle);
			return;
		}
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		BeforeJumpState = EPlayState::Move;
		StateChange(EPlayState::Jump);
		return;
	}
}

// Crouch : ��ũ����
void APlayer::Crouch(float _DeltaTime)
{
	// ��ũ�� ���¿����� ���� ��ȯ������ ����� ��.
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));

	// ��ũ���� ���¿��� ����Ű ������ �����̵�
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayState::Slide);
		return;
	}

	if (true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EPlayState::Idle);
		return;
	}
}

// Slide : �����̵�
void APlayer::Slide(float _DeltaTime)
{
	// �����̵��� MoveVector�� üũ
	if (abs(MoveVector.X) < 10.0f)
	{
		MoveVector = FVector::Zero;
		StateChange(EPlayState::Idle);
		return;
	}
	else 
	{
		AddMoveVector({ (-1.0f) * static_cast<float>(DirState) * _DeltaTime, 0.f }, SlideAcc);
		MoveUpdate(_DeltaTime, SlideMaxSpeed, SlideAcc);
		return;
	}
}

// Run : �޸���
void APlayer::Run(float _DeltaTime)
{	
	if (DirCheck())
	{
		if (IsPlayerBottomMagentaA())
		{
			// �� ������ ���� �ٲ������ �극��ũ
			StateChange(EPlayState::Break);
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
		StateChange(EPlayState::Idle);
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
			StateChange(EPlayState::Idle);
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
			StateChange(EPlayState::Idle);
			return;
		}
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		BeforeJumpState = EPlayState::Run;
		StateChange(EPlayState::Jump);
		return;
	}
}

// Jump : ����
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

	if (abs(FinalMoveVector.Y) < 40.f)
	{
		PlayerRenderer->ChangeAnimation(GetAnimationName("JumpTurn"));
	}

	if (IsPlayerBottomMagentaA() || IsPlayerBottomYellow())
	{
		JumpVector = FVector::Zero;
		StateChange(BeforeJumpState);
		return;
	}
}

void APlayer::Break(float _DeltaTime)
{
	if (PlayerRenderer->IsCurAnimationEnd()) 
	{
		StateChange(EPlayState::Idle);
		return;
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
		InhaleCollision->SetScale({ InhaleScaleVar + 10.f, InhaleScaleVar });

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
	if (true == BodyCollision->CollisionCheck(KirbyCollisionOrder::Monster, Result))
	{
		// ���� �Ծ������
	}

	// �� �ʵ��� ���Կ� �� �� ������ ���� �׳� ����.. ���߿� ����.
	// ���� ����
	if (true == UEngineInput::IsFree('X'))
	{
		InhaleCollision->ActiveOff();
		StateChange(EPlayState::Idle);
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
		StateChange(EPlayState::Idle);
	}
}


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
		StateChange(EPlayState::Idle);
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