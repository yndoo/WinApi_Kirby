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
	PlayerRenderer->CreateAnimation("Idle_Right", "Idle_Right.png", { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2 }, 0.09f, true);
	PlayerRenderer->CreateAnimation("Idle_Left", "Idle_Left.png", { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2 }, 0.09f, true);
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
		// 가만히
		Idle(_DeltaTime);
		break;
	case EPlayState::Move:
		// 이동
		Move(_DeltaTime);
		break;
	case EPlayState::Crouch:
		// 웅크리기
		Crouch(_DeltaTime);
		break;
	case EPlayState::Slide:
		// 슬라이드
		Slide(_DeltaTime);
		break;
	case EPlayState::Run:
		// 달리기
		Run(_DeltaTime);
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
		// 같은 방향 더블 클릭이라면 Run
		if (IsMoveClicked == true && !DirCheck() && MoveDoubleClickTime < 0.15f)	
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
	//MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);
}

// Move : 플레이어 이동(오른쪽 왼쪽)
void APlayer::Move(float _DeltaTime) {
	DirCheck();
	GravityCheck(_DeltaTime);

	MoveDoubleClickTime += _DeltaTime;
	// 입력에 의한 이동 계산
	if (DirState == EActorDir::Left)
	{
		AddMoveVector(FVector::Left * _DeltaTime);
	}

	if (DirState == EActorDir::Right)
	{
		AddMoveVector(FVector::Right * _DeltaTime);
	}

	// Move하다가 가만히 있는 경우
	if (true == UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);	
		IsMoveClicked = true;
		return;
	}

	PlayerRenderer->ChangeAnimation(GetAnimationName("Move"));
	MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);
}

// Crouch : 웅크리기
void APlayer::Crouch(float _DeltaTime)
{
	GravityCheck(_DeltaTime);

	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));

	// 웅크리기 상태에서 점프키 누르면 슬라이드
	if (true == UEngineInput::IsDown('Z'))
	{
		SlideTime = 0;
		MoveVector = FVector::Zero;
		if (DirState == EActorDir::Left)
		{
			AddMoveVector(FVector::Left * _DeltaTime * 400);
		}
		if (DirState == EActorDir::Right)
		{
			AddMoveVector(FVector::Right * _DeltaTime * 400);
		}
		StateChange(EPlayState::Slide);

		return;
	}

	if (true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EPlayState::Idle);
		return;
	}
}

// Slide : 슬라이딩
void APlayer::Slide(float _DeltaTime)
{
	GravityCheck(_DeltaTime);
	DirCheck();

	PlayerRenderer->ChangeAnimation(GetAnimationName("Slide"));

	if (SlideTime < 0.5f)
	{
		SlideTime += _DeltaTime;
		MoveUpdate(_DeltaTime, SlideMaxSpeed, SlideAcc);
		return;
	}
	else 
	{
		if (true == UEngineInput::IsFree(VK_DOWN))
		{
			StateChange(EPlayState::Idle);
			return;
		}
		else if (true == UEngineInput::IsPress(VK_DOWN))
		{
			StateChange(EPlayState::Crouch);
			return;
		}
	}

}

// Run : 달리기
void APlayer::Run(float _DeltaTime)
{
	MoveDoubleClickTime = 0;
	IsMoveClicked = false;

	DirCheck();
	GravityCheck(_DeltaTime);

	if (DirState == EActorDir::Left)
	{
		AddMoveVector(FVector::Left * _DeltaTime);
	}

	if (DirState == EActorDir::Right)
	{
		AddMoveVector(FVector::Right * _DeltaTime);
	}

	if (true == UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);
		return;
	}

	MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);
	PlayerRenderer->ChangeAnimation(GetAnimationName("Run"));
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

	if (Dir != DirState)	// 방향 변경됐으면 애니메이션 다시
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

// MoveUpdate : 작용하는 힘 계산해서 이동
void APlayer::MoveUpdate(float _DeltaTime, float MaxSpeed, FVector Acc)
{
	// 모든 작용하는 힘 계산해서 이동
	CalMoveVector(_DeltaTime, MaxSpeed, Acc);
	CalGravityVector(_DeltaTime);
	CalFinalMoveVector(_DeltaTime);
	FinalMove(_DeltaTime);

	// 경사로 이동
	HillMove(_DeltaTime);
}

void APlayer::AddMoveVector(const FVector& _DirDelta) {
	MoveVector += _DirDelta * MoveAcc;
}

void APlayer::CalMoveVector(float _DeltaTime, float MaxSpeed, FVector Acc)
{
	FVector CheckPos = GetActorLocation();
	switch (DirState) 
	{
	case EActorDir::Left:
		CheckPos.X -= 20;
		break;
	case EActorDir::Right:
		CheckPos.X += 20;
		break;
	default:
		break;
	}
	CheckPos.Y -= 40;	// 40픽셀 이상 높이의 언덕맵은 막히게 하기 (커비 잔디 한 블록이 40픽셀)

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		MoveVector = FVector::Zero;
	}

	if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT)) 
	{
		if (0.001 <= MoveVector.Size2D())
		{
			// 움직이던 방향 반대로 가속도
			MoveVector += (-MoveVector.Normalize2DReturn()) * _DeltaTime * Acc;
		}
		else 
		{
			MoveVector = float4::Zero;
		}
	}
	// 최대 속도를 넘어가지 않도록
	if (MaxSpeed <= MoveVector.Size2D())
	{
		MoveVector = MoveVector.Normalize2DReturn() * MaxSpeed;
	}
}

void APlayer::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime;

}

void APlayer::CalFinalMoveVector(float _DeltaTime)
{
	FinalMoveVector = FVector::Zero;
	FinalMoveVector += MoveVector;
	FinalMoveVector += GravityVector;
}

// 최종 계산된 방향과 힘으로 이동시키는 함수
void APlayer::FinalMove(float _DeltaTime) 
{
	FVector MovePos = FinalMoveVector * _DeltaTime;						// 플레이어 이동량 (걷기의 Move가 아님)

	// 플레이어 이동
	AddActorLocation(MovePos);

	// 카메라 이동
	FVector CameraPosLeft = GetWorld()->GetCameraPos();					// 카메라 왼쪽 좌표
	FVector NextCameraPosLeft = CameraPosLeft + MovePos;				// 플레이어 이동 후 왼쪽 카메라 끝 
	FVector NextCameraPosRight = WinScale + NextCameraPosLeft;			// 이동 후의 카메라 오른쪽 끝
	FVector NextPlayerPos = GetActorLocation();							// 플레이어 위치

	if (
		NextPlayerPos.X >= NextCameraPosLeft.X + WinScale.hX() &&		// 플레이어 위치가 맵 왼쪽 끝에서 절반 이상일 때부터 카메라 이동하도록
		NextPlayerPos.X <= 4720 - WinScale.hX() &&						// 플레이어 위치가 맵 오른쪽 끝에서 절반일 때까지만 카메라 따라오도록
		NextCameraPosLeft.X >= 0 &&										// 카메라가 맵 밖으로 안 나오도록
		NextCameraPosRight.X <= 4720
		)
	{
		GetWorld()->AddCameraPos(MoveVector * _DeltaTime);
	}
}