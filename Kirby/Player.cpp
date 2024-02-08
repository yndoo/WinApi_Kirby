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
	
	
	// 이미지보다 크게 자르면 아예 안 나옴.
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
		// 가만히
		Idle(_DeltaTime);
		break;
	case EPlayState::Move:
		// 이동
		Move(_DeltaTime);
		break;
	case EPlayState::FreeMove:
		// 자유 이동
		FreeMove(_DeltaTime);
		break;
	case EPlayState::CameraFreeMove:
		// 카메라 자유 이동
		CameraFreeMove(_DeltaTime);
		break;
	default:
		break;
	}
}

// Idle : 가만히 있는 상태
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

// Move : 맵 이동(오른쪽 왼쪽)
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
		StateChange(EPlayState::Idle);	// Move하다가 가만히 있는 경우
		return;
	}

	FVector CurLoc = GetActorLocation();
	FVector NextLoc = CurLoc + MovePos;

	// 경사 코드
	// : 이동할 앞쪽 위치를 1픽셀씩 올려보면서 마젠타가 아닐 때까지의 픽셀 값(i)을 구해서 플레이어가 움직일 위치를 i만큼 올려준다.

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

	if (NextLoc.X <= 0 && NextLoc.X >= 4720) {	// 경사체크 하기 전에 맵 넘어가는거 미리 체크. 안 하면 무한루프 걸림.
		return;
	}

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(NextLoc.iX(), NextLoc.iY() - 2, Color8Bit::MagentaA);
	int i = 0;
 	while (Color == Color8Bit(255, 0, 255, 0)) {
		i++;
		Color = UContentsHelper::ColMapImage->GetColor(NextLoc.iX(), NextLoc.iY() - i, Color8Bit::MagentaA);
	}

	// while문을 나오면 경사 높이 알 수 있음
	MovePos.Y -= i;

	if (Color != Color8Bit(255, 0, 255, 0))
	{
		// 이동 가능한 경우
		
		// 플레이어도 맵을 못 나가도록 조건. 4720은 1-3의 첫번째 맵 가로크기
		if (CurLoc.X + MovePos.X > 0 && CurLoc.X + MovePos.X < 4720) {
			AddActorLocation(MovePos);
			
			//MovePos.Y += i;				// 카메라는 올라가면 안 되니까 다시 더해줌
		}

		// 카메라가 플레이어 따라오는 문제 해결 코드
		FVector NextCameraPosLeft = GetWorld()->GetCameraPos() + MovePos;	// 왼쪽 카메라 끝
		FVector WinScale = GEngine->MainWindow.GetWindowScale();			// 윈도우 크기
		FVector NextCameraPosRight = WinScale + NextCameraPosLeft;			// 오른쪽 카메라 끝
		FVector NextPlayerPos = GetActorLocation();							// 현재 플레이어 위치


		if (
			NextPlayerPos.X >= NextCameraPosLeft.X + WinScale.hX() &&		// 플레이어 위치가 맵 왼쪽 끝에서 절반 이상일 때부터 카메라 이동하도록
			NextPlayerPos.X <= 4720 - WinScale.hX() &&						// 플레이어 위치가 맵 오른쪽 끝에서 절반일 때까지만 카메라 따라오도록
			NextCameraPosLeft.X >= 0 &&										// 카메라가 맵 밖으로 안 나오도록
			NextCameraPosRight.X <= 4720 &&
			NextCameraPosLeft.Y >= 0
			) {
			// MovePos가 맵 밖이 아니면
			GetWorld()->AddCameraPos(MovePos);
		}
	}

	PlayerRenderer->ChangeAnimation(GetAnimationName("Move")); 
}

// FreeMove : 디버깅용 캐릭터 자유 이동
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

// 움직이고 있는 방향 알려줌
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

	if (Dir != DirState)	// 방향 변경됐으면 애니메이션 다시
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);
		PlayerRenderer->ChangeAnimation(Name);
	}
}