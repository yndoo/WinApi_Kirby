#include "Player.h"
#include<EnginePlatform/EngineInput.h>
#include "ContentsHelper.h"
#include <EngineCore/EngineCore.h>

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::AutoCreateAnimation(std::string_view _AnimationName, /*std::string_view _ImageName,*/ std::vector<int> _Indexs, float _Inter, bool _Loop)
{
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Right"), std::string(_AnimationName) + std::string("_Right.png"), _Indexs, _Inter, _Loop);
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Left"), std::string(_AnimationName) + std::string("_Left.png"), _Indexs, _Inter, _Loop);
}
void APlayer::AutoCreateAnimation(std::string_view _AnimationName, /*std::string_view _ImageName,*/ int _Start, int _End, float _Inter, bool _Loop)
{
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Right"), std::string(_AnimationName) + std::string("_Right.png"), _Start, _End, _Inter, _Loop);
	PlayerRenderer->CreateAnimation(std::string(_AnimationName) + std::string("_Left"), std::string(_AnimationName) + std::string("_Left.png"), _Start, _End, _Inter, _Loop);
}

void APlayer::BeginPlay() {
	AActor::BeginPlay();

	SetActorLocation({ 100,100 });

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

	// ���� �̹����� �� �ִϸ��̼��� �����ϱ� ������ Auto�� �Ұ���
	PlayerRenderer->CreateAnimation("JumpTurn_Right", "Jump_Right.png", 1, 8, 0.05f, false);	// ���� ȸ��
	PlayerRenderer->CreateAnimation("JumpTurn_Left", "Jump_Left.png", 1, 8, 0.05f, false);
	PlayerRenderer->CreateAnimation("JumpStart_Right", "Jump_Right.png", 0, 0, 0.1f, false);	// ���� ����
	PlayerRenderer->CreateAnimation("JumpStart_Left", "Jump_Left.png", 0, 0, 0.1f, false);

	PlayerRenderer->ChangeAnimation("Idle_Right");
	StateChange(EPlayState::Idle);
}

void APlayer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
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
		MoveVector = { static_cast<float>(DirState) * 50, 0.f };
	}
}

void APlayer::RunStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Run"));
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
	// Jump ���ҽ� ���� ������ ���� ******
	PlayerRenderer->ChangeAnimation(GetAnimationName("JumpStart"));
}

void APlayer::BreakStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Break"));
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
	if (DirCheck()) 
	{
		// ���� �ٲ������ �극��ũ
		StateChange(EPlayState::Break);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) && true == UEngineInput::IsPress(VK_RIGHT))
	{
		// ���ÿ� �Էµ��� ���� Idle
		StateChange(EPlayState::Idle);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
	{
		// �Է¿� ���� �̵� ���
		AddMoveVector({ static_cast<float>(DirState) * _DeltaTime, 0.f }, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);
	}
	else if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// ����
		FVector DirDelta = FVector((-1.0f) * static_cast<float>(DirState) * _DeltaTime, 0.f);
		AddMoveVector(DirDelta, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);

		// ���� �ӵ� ���ϸ� ���߱�
		if (abs(FinalMoveVector.X) < 50.0f)
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
	if (abs(FinalMoveVector.X) < 10.0f)
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
		// ���� �ٲ������ �극��ũ
		StateChange(EPlayState::Break);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) && true == UEngineInput::IsPress(VK_RIGHT))
	{
		// ���ÿ� �Էµ��� ���� Idle
		StateChange(EPlayState::Idle);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
	{
		// �Է¿� ���� �̵� ���
		AddMoveVector({ static_cast<float>(DirState) * _DeltaTime, 0.f }, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);
	}
	else if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// ����
		FVector DirDelta = FVector((-1.0f) * static_cast<float>(DirState) * _DeltaTime, 0.f);
		AddMoveVector(DirDelta, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);
		// ���� �ӵ� ���ϸ� ���߱�
		if (abs(FinalMoveVector.X) < 10.0f)
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

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		DirCheck();
		AddMoveVector({ static_cast<float>(DirState) * _DeltaTime, 0.f }, MoveAcc); // ���� �� �̵��� Move���ӵ���
	}

	MoveUpdate(_DeltaTime, JumpMaxSpeed);

	if (abs(FinalMoveVector.Y) < 50.f)
	{
		PlayerRenderer->ChangeAnimation(GetAnimationName("JumpTurn"));
	}

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
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

void APlayer::HillMove(float _DeltaTime)
{
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color == Color8Bit(255, 0, 255, 0))
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

	// ���� �̵�
	HillMove(_DeltaTime);
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

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		GravityVector = FVector::Zero;
	}
	int a = 0;
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
	FVector MovePos = FinalMoveVector * _DeltaTime;					// �÷��̾� �̵��� (�ȱ��� Move�� �ƴ�)

	FVector CheckPos = GetActorLocation();
	CheckPos.X += static_cast<float>(DirState) * 20.0f;	// �յڷ� 20�ȼ�
	CheckPos.Y -= 20;									// �ܵ� ��� ������

	FVector CheckPos2 = GetActorLocation();
	CheckPos2.X += static_cast<float>(DirState) * 20.0f;	// �յڷ� 20�ȼ�
	CheckPos2.Y -= 28;										// ���δ� �ö�ߵ�

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	Color8Bit Color2 = UContentsHelper::ColMapImage->GetColor(CheckPos2.iX(), CheckPos2.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA && Color2 == Color8Bit::MagentaA)
	{
		return;
	}
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