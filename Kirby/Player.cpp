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

	PlayerRenderer->CreateAnimation("Move_Right", "Move_Right.png", 0, 9, 0.07f, true);
	PlayerRenderer->CreateAnimation("Move_Left", "Move_Left.png", 0, 9, 0.07f, true);
	PlayerRenderer->CreateAnimation("Idle_Right", "Idle_Right.png", { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2 }, 0.09f, true);
	PlayerRenderer->CreateAnimation("Idle_Left", "Idle_Left.png", { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2 }, 0.09f, true);
	PlayerRenderer->CreateAnimation("Crouch_Right", "Crouch_Right.png", 1, 1, 0.3f, true);
	PlayerRenderer->CreateAnimation("Crouch_Left", "Crouch_Left.png", 1, 1, 0.3f, true);
	PlayerRenderer->CreateAnimation("Slide_Right", "Slide_Right.png", 0, 0, 0.3f, true);
	PlayerRenderer->CreateAnimation("Slide_Left", "Slide_Left.png", 0, 0, 0.3f, true);
	PlayerRenderer->CreateAnimation("Run_Right", "Run_Right.png", 0, 7, 0.05f, true);
	PlayerRenderer->CreateAnimation("Run_Left", "Run_Left.png", 0, 7, 0.05f, true);

	PlayerRenderer->ChangeAnimation("Idle_Right");


	StateChange(EPlayState::Idle);

	int a = 0;

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
		default:
			break;
		}
	}
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

void APlayer::IdleStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Idle"));
}

void APlayer::MoveStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Move"));
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

// Idle : 가만히 있는 상태
void  APlayer::Idle(float _DeltaTime) {
	MoveVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown(VK_LEFT) || true == UEngineInput::IsDown(VK_RIGHT))
	{
		// 그냥 Move
		StateChange(EPlayState::Move);
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
}

// Move : 플레이어 이동(오른쪽 왼쪽)
void APlayer::Move(float _DeltaTime) 
{
	if (DirCheck()) // 방향 바뀌었으면 
	{
		// 브레이크모션(일단 Idle)
		StateChange(EPlayState::Idle);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) && true == UEngineInput::IsPress(VK_RIGHT))
	{
		// 동시에 입력됐을 때는 Idle
		StateChange(EPlayState::Idle);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
	{
		// 입력에 의한 이동 계산
		AddMoveVector({ static_cast<float>(DirState) * _DeltaTime, 0.f }, MoveAcc);
	}
	else if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// 감속
		FVector DirDelta = FVector((-1.0f) * static_cast<float>(DirState) * _DeltaTime, 0.f);
		AddMoveVector(DirDelta, MoveAcc);
		// 일정 속도 이하면 멈추기
		if (abs(MoveVector.X) < 1.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EPlayState::Idle);
			return;
		}
	}
	MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);
}

// Crouch : 웅크리기
void APlayer::Crouch(float _DeltaTime)
{
	// 웅크린 상태에서의 방향 전환때문에 해줘야 함.
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Crouch"));

	// 웅크리기 상태에서 점프키 누르면 슬라이드
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

// Slide : 슬라이딩
void APlayer::Slide(float _DeltaTime)
{
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

// Run : 달리기
void APlayer::Run(float _DeltaTime)
{	
	if (DirCheck()) // 방향 바뀌었으면 
	{
		// 브레이크모션(일단 Idle)
		StateChange(EPlayState::Idle);
		return;
	}
	if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
	{
		// 입력에 의한 이동 계산
		DirCheck();
		PlayerRenderer->ChangeAnimation(GetAnimationName("Move"));
		AddMoveVector({ static_cast<float>(DirState) * _DeltaTime, 0.f }, RunAcc);
	}
	else if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// 입력 끝나면 감속
		AddMoveVector({ (-1.0f) * static_cast<float>(DirState) * _DeltaTime, 0.f }, RunAcc);
		if (abs(MoveVector.X) < 3.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EPlayState::Idle);
			return;
		}
	}

	MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);
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
void APlayer::MoveUpdate(float _DeltaTime, float MaxSpeed/* = 0.0f*/, FVector Acc /*= FVector::Zero*/)
{
	// MaxSpeed, Acc 입력 없었으면 CalMoveVector 하면 안 됨
	if (MaxSpeed != 0.0f) 
	{
		CalMoveVector(_DeltaTime, MaxSpeed, Acc);
	}
	CalGravityVector(_DeltaTime);
	CalFinalMoveVector(_DeltaTime);
	FinalMove(_DeltaTime);

	// 경사로 이동
	HillMove(_DeltaTime);
}

void APlayer::AddMoveVector(const FVector& _DirDelta, FVector Acc)
{
	MoveVector += _DirDelta * Acc;
}

void APlayer::CalMoveVector(float _DeltaTime, float MaxSpeed, FVector Acc)
{
	// 벽 못가게 체크
	FVector CheckPos = GetActorLocation();
	CheckPos.X += static_cast<float>(DirState) * 20.0f;	// 앞뒤로 20픽셀
	CheckPos.Y -= 20;									// 잔디 블록 막히게

	FVector CurPos = GetActorLocation();
	CurPos.X += static_cast<float>(DirState) * 20.0f;	// 앞뒤로 20픽셀
	CurPos.Y -= 28;										// 경사로는 올라야돼

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	Color8Bit Color2 = UContentsHelper::ColMapImage->GetColor(CurPos.iX(), CurPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA && Color2 == Color8Bit::MagentaA)
	{
		MoveVector = FVector::Zero;
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
}

// 최종 계산된 방향과 힘으로 이동시키는 함수
void APlayer::FinalMove(float _DeltaTime) 
{
	FVector MovePos = FinalMoveVector * _DeltaTime;					// 플레이어 이동량 (걷기의 Move가 아님)

	FVector CheckPos = GetActorLocation();
	CheckPos.X += static_cast<float>(DirState) * 20.0f;	// 앞뒤로 20픽셀
	CheckPos.Y -= 20;									// 잔디 블록 막히게

	FVector CheckPos2 = GetActorLocation();
	CheckPos2.X += static_cast<float>(DirState) * 20.0f;	// 앞뒤로 20픽셀
	CheckPos2.Y -= 28;										// 경사로는 올라야돼

	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	Color8Bit Color2 = UContentsHelper::ColMapImage->GetColor(CheckPos2.iX(), CheckPos2.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit::MagentaA && Color2 == Color8Bit::MagentaA)
	{
		return;
	}
	// 플레이어 이동
	AddActorLocation(MovePos);

	// 카메라 이동
	FVector CameraPosLeft = GetWorld()->GetCameraPos();				// 카메라 왼쪽 좌표
	FVector NextCameraPosLeft = CameraPosLeft + MovePos;			// 플레이어 이동 후 왼쪽 카메라 끝 
	FVector NextCameraPosRight = WinScale + NextCameraPosLeft;		// 이동 후의 카메라 오른쪽 끝
	FVector NextPlayerPos = GetActorLocation();								// 플레이어 위치

	

	if (
		NextPlayerPos.X >= NextCameraPosLeft.X + WinScale.hX() &&	// 플레이어 위치가 맵 왼쪽 끝에서 절반 이상일 때부터 카메라 이동하도록
		NextPlayerPos.X <= MapSize.X - WinScale.hX() &&				// 플레이어 위치가 맵 오른쪽 끝에서 절반일 때까지만 카메라 따라오도록
		NextCameraPosLeft.X >= 0 &&									// 카메라가 맵 밖으로 안 나오도록
		NextCameraPosRight.X <= MapSize.X
		)
	{
		GetWorld()->AddCameraPos(MoveVector * _DeltaTime);
	}
}