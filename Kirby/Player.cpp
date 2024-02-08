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
		StateChange(EPlayState::Move);
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

// Move : �� �̵�(������ ����)
void APlayer::Move(float _DeltaTime) {
	DirCheck();
	GravityCheck(_DeltaTime);

	FVector MovePos = FVector::Zero;
	if (UEngineInput::IsPress(VK_LEFT))
	{
		MovePos += FVector::Left * _DeltaTime * MoveSpeed;
	}

	if (UEngineInput::IsPress(VK_RIGHT))
	{
		MovePos += FVector::Right * _DeltaTime * MoveSpeed;
	}

	if (true == UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);	// Move�ϴٰ� ������ �ִ� ���
		return;
	}

	FVector CurLoc = GetActorLocation();
	FVector NextLoc = CurLoc + MovePos;

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

	if (NextLoc.X <= 0 && NextLoc.X >= 4720) {	// ���üũ �ϱ� ���� �� �Ѿ�°� �̸� üũ. �� �ϸ� ���ѷ��� �ɸ�.
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
		if (CurLoc.X + MovePos.X > 0 && CurLoc.X + MovePos.X < 4720) {
			AddActorLocation(MovePos);
			
			//MovePos.Y += i;				// ī�޶�� �ö󰡸� �� �Ǵϱ� �ٽ� ������
		}

		// ī�޶� �÷��̾� ������� ���� �ذ� �ڵ�
		FVector NextCameraPosLeft = GetWorld()->GetCameraPos() + MovePos;	// ���� ī�޶� ��
		FVector WinScale = GEngine->MainWindow.GetWindowScale();			// ������ ũ��
		FVector NextCameraPosRight = WinScale + NextCameraPosLeft;			// ������ ī�޶� ��
		FVector NextPlayerPos = GetActorLocation();							// ���� �÷��̾� ��ġ


		if (
			NextPlayerPos.X >= NextCameraPosLeft.X + WinScale.hX() &&		// �÷��̾� ��ġ�� �� ���� ������ ���� �̻��� ������ ī�޶� �̵��ϵ���
			NextPlayerPos.X <= 4720 - WinScale.hX() &&						// �÷��̾� ��ġ�� �� ������ ������ ������ �������� ī�޶� ���������
			NextCameraPosLeft.X >= 0 &&										// ī�޶� �� ������ �� ��������
			NextCameraPosRight.X <= 4720 &&
			NextCameraPosLeft.Y >= 0
			) {
			// MovePos�� �� ���� �ƴϸ�
			GetWorld()->AddCameraPos(MovePos);
		}
	}

	PlayerRenderer->ChangeAnimation(GetAnimationName("Move")); 
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
void APlayer::DirCheck()
{
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
	}
}