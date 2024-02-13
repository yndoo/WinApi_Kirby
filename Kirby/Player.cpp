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

void APlayer::BeginPlay() {
	AActor::BeginPlay();

	SetActorLocation({ 100,100 });

	PlayerRenderer = CreateImageRenderer(0);
	PlayerRenderer->SetImage("Kirby.png");

	PlayerRenderer->SetTransform({ {0,0}, {300, 300} });
	PlayerRenderer->SetTransColor(Color8Bit::Blue);


	// �̹������� ũ�� �ڸ��� �ƿ� �� ����.
	//PlayerRenderer->SetImageCuttingTransform({ {0,0}, {60, 80} });

	PlayerRenderer->CreateAnimation("Move_Right", "Move_Right.png", 0, 9, 0.1f, true);
	PlayerRenderer->CreateAnimation("Move_Left", "Move_Left.png", 0, 9, 0.1f, true);
	PlayerRenderer->CreateAnimation("Idle_Right", "Idle_Right.png", 0, 2, 0.3f, true);
	PlayerRenderer->CreateAnimation("Idle_Left", "Idle_Left.png", 0, 2, 0.3f, true);
	PlayerRenderer->CreateAnimation("Crouch_Right", "Crouch_Right.png", 1, 1, 0.3f, true);
	PlayerRenderer->CreateAnimation("Crouch_Left", "Crouch_Left.png", 1, 1, 0.3f, true);
	PlayerRenderer->CreateAnimation("Slide_Right", "Slide_Right.png", 0, 0, 0.3f, true);
	PlayerRenderer->CreateAnimation("Slide_Left", "Slide_Left.png", 0, 0, 0.3f, true);
	PlayerRenderer->CreateAnimation("Run_Right", "Run_Right.png", 0, 7, 0.1f, true);
	PlayerRenderer->CreateAnimation("Run_Left", "Run_Left.png", 0, 7, 0.1f, true);

	PlayerRenderer->ChangeAnimation("Idle_Right");


	StateChange(EPlayState::Idle);

	int a = 0;

}

void APlayer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void APlayer::GravityCheck(float _DeltaTime)
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color != Color8Bit(255, 0, 255, 0))
	{
		AddActorLocation(FVector::Down * _DeltaTime * Gravity);
	}
}

void APlayer::StateChange(EPlayState _State) {
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

// Idle : ������ �ִ� ����
void  APlayer::Idle(float _DeltaTime) {
	if (
		true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT)
		)
	{
		if (IsMoveClicked == true && !DirCheck() && MoveDoubleClickTime < 0.3f)
		{
			StateChange(EPlayState::Run);
			return;
		}
		StateChange(EPlayState::Move);
		MoveDoubleClickTime = 0;
		return;
	}

	if (true == UEngineInput::IsPress(VK_DOWN))
	{
		StateChange(EPlayState::Crouch);
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

	GravityCheck(_DeltaTime);
	PlayerRenderer->ChangeAnimation(GetAnimationName("Idle"));
}

// Move : �÷��̾� �̵�(������ ����)
void APlayer::Move(float _DeltaTime) {
	DirCheck();
	GravityCheck(_DeltaTime);

	MoveDoubleClickTime += _DeltaTime;

	RealMove(_DeltaTime, MoveSpeed);

	if (true == UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);	// Move�ϴٰ� ������ �ִ� ���
		IsMoveClicked = true;
		return;
	}

	PlayerRenderer->ChangeAnimation(GetAnimationName("Move"));
}

// Crouch : ��ũ����
void APlayer::Crouch(float _DeltaTime)
{
	GravityCheck(_DeltaTime);

	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));

	// ��ũ���� ���¿��� ����Ű ������ �����̵�
	if (true == UEngineInput::IsPress('Z'))
	{
		SlideTime = 0;
		StateChange(EPlayState::Slide);
		return;
	}

	if (true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EPlayState::Idle);
		return;
	}
}

void APlayer::Slide(float _DeltaTime)
{
	GravityCheck(_DeltaTime);

	RealMove(_DeltaTime, SlideSpeed);

	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Slide"));

	SlideTime += _DeltaTime;

	if (SlideTime < 0.5f)
	{
		return;
	}
	else {
		if (true == UEngineInput::IsFree(VK_DOWN))
		{
			StateChange(EPlayState::Idle);
			return;
		}
		else {
			StateChange(EPlayState::Crouch);
			return;
		}
	}


}

// Run : �޸���
void APlayer::Run(float _DeltaTime)
{
	MoveDoubleClickTime = 0;
	IsMoveClicked = false;

	DirCheck();
	GravityCheck(_DeltaTime);

	RealMove(_DeltaTime, RunSpeed);

	if (true == UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);
		return;
	}
	PlayerRenderer->ChangeAnimation(GetAnimationName("Run"));
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

// RealMove : ��¥ �̵���Ű�� �Լ�
void APlayer::RealMove(float _DeltaTime, float _MoveSpeed)
{
	// MovePos : �̵��� ��
	FVector MovePos = FVector::Zero;
	if (DirState == EActorDir::Left)
	{
		MovePos += FVector::Left * _DeltaTime * _MoveSpeed;
	}

	if (DirState == EActorDir::Right)
	{
		MovePos += FVector::Right * _DeltaTime * _MoveSpeed;
	}

	FVector CurLoc = GetActorLocation();
	FVector NextLoc = CurLoc + MovePos;		// �̵��� ��ġ

	// ��� �ڵ�
	// : �̵��� ���� ��ġ�� 1�ȼ��� �÷����鼭 ����Ÿ�� �ƴ� �������� �ȼ� ��(i)�� ���ؼ� �÷��̾ ������ ��ġ�� i��ŭ �÷��ش�.

	switch (DirState)
	{
	case EActorDir::Left:
		NextLoc.X -= 20;
		break;
	case EActorDir::Right:
		NextLoc.X += 20;
		break;
	default:
		break;
	}

	if (NextLoc.X < 0 || NextLoc.X > 4718) {	// ���üũ �ϱ� ���� �� �Ѿ�°� �̸� üũ. �� �ϸ� ���ѷ��� �ɸ�.
		return;
	}

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(NextLoc.iX(), NextLoc.iY() - 2, Color8Bit::MagentaA);
	int i = 0;
	while (Color == Color8Bit(255, 0, 255, 0)) {
		i++;
		Color = UContentsHelper::ColMapImage->GetColor(NextLoc.iX(), NextLoc.iY() - i, Color8Bit::MagentaA);
	} 
	// while���� ������ ��� ���� �� �� ����
	MovePos.Y -= i;

	if (Color != Color8Bit(255, 0, 255, 0))
	{
		// �̵� ������ ���

		// �÷��̾ ���� �� �������� ����. 4720�� 1-3�� ù��° �� ����ũ��
		if (NextLoc.X >= 0 && NextLoc.X <= 4718) {
			AddActorLocation(MovePos);	// ��¥ �̵���Ŵ

			MovePos.Y += i;				// ī�޶�� �ö󰡸� �� �Ǵϱ� �ٽ� ������
		}

		// ī�޶� �÷��̾� ������� ���� �ذ� �ڵ�
		FVector CameraPosLeft = GetWorld()->GetCameraPos();					// ī�޶� ���� ��
		FVector NextCameraPosLeft = GetWorld()->GetCameraPos() + MovePos;	// ���� ī�޶� �� + MovePos (�̵� ���� ī�޶� ��ġ, ����)
		//FVector WinScale = GEngine->MainWindow.GetWindowScale();			// ������ ũ��
		FVector NextCameraPosRight = WinScale + NextCameraPosLeft;			// �̵� ���� ī�޶� ������ ��
		FVector NextPlayerPos = GetActorLocation();							// �̵��� �÷��̾� ��ġ

		float Test = 4720 - WinScale.hX();

		if (
			NextPlayerPos.X >= NextCameraPosLeft.X + WinScale.hX() &&		// �÷��̾� ��ġ�� �� ���� ������ ���� �̻��� ������ ī�޶� �̵��ϵ���
			NextPlayerPos.X <= 4720 - WinScale.hX() &&						// �÷��̾� ��ġ�� �� ������ ������ ������ �������� ī�޶� ���������
			NextCameraPosLeft.X >= 0 &&										// ī�޶� �� ������ �� ��������
			NextCameraPosRight.X <= 4720
			) 
		{
			// MovePos�� �� ���� �ƴϸ�
			GetWorld()->AddCameraPos(MovePos);
		}

		FVector Test2 = GetWorld()->GetCameraPos();
		int a = 0;
	}
}
