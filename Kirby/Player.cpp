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

	// 같은 이미지로 여러 애니메이션을 제작하기 때문에 직접 해줌
	PlayerRenderer->CreateAnimation("JumpTurn_Right", "Jump_Right.png", 1, 8, 0.05f, false);	// 공중 회전
	PlayerRenderer->CreateAnimation("JumpTurn_Left", "Jump_Left.png", 1, 8, 0.05f, false);
	PlayerRenderer->CreateAnimation("JumpStart_Right", "Jump_Right.png", 0, 0, 0.1f, false);	// 점프 시작
	PlayerRenderer->CreateAnimation("JumpStart_Left", "Jump_Left.png", 0, 0, 0.1f, false);

	PlayerRenderer->ChangeAnimation("Idle_Right");
	StateChange(EPlayState::Idle);

	BodyCollision = CreateCollision(KirbyCollisionOrder::Player);
	BodyCollision->SetScale({ 10, 100 });
	BodyCollision->SetColType(ECollisionType::CirCle);
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
	case EPlayState::Jump:
		// 점프
		Jump(_DeltaTime);
		break;
	case EPlayState::Break:
		// 브레이크
		Break(_DeltaTime);
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
	PlayerRenderer->ChangeAnimation(GetAnimationName("JumpStart"));
}

void APlayer::BreakStart()
{
	DirCheck();
	PlayerRenderer->ChangeAnimation(GetAnimationName("Break"));
}

// Idle : 가만히 있는 상태
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
		// 그냥 Move
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

// Move : 플레이어 이동(오른쪽 왼쪽)
void APlayer::Move(float _DeltaTime)
{
	if (DirCheck())	// 방향 전환됐을 경우
	{
		if (IsPlayerBottomMagentaA())
		{
			// 땅 위에서 방향 바뀌었으면 브레이크
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
		// 동시에 입력됐을 때는 Idle
		StateChange(EPlayState::Idle);
		return;
	}

	// 입력에 의한 이동 계산
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

	// 걷는 중에 연타 시 달리기
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
		// 감속
		AddMoveVector(FVector::Right * _DeltaTime, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);

		// 일정 속도 이하면 멈추기
		if (FinalMoveVector.X > -80.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EPlayState::Idle);
			return;
		}
	}
	if (MyDir == EActorDir::Right && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// 감속
		AddMoveVector(FVector::Left * _DeltaTime, MoveAcc);
		MoveUpdate(_DeltaTime, MoveMaxSpeed, MoveAcc);

		// 일정 속도 이하면 멈추기
		if (FinalMoveVector.X < 80.0f)
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
	// 슬라이딩은 MoveVector로 체크
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
	if (DirCheck())
	{
		if (IsPlayerBottomMagentaA())
		{
			// 땅 위에서 방향 바뀌었으면 브레이크
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
		// 동시에 입력됐을 때는 Idle
		StateChange(EPlayState::Idle);
		return;
	}

	// 입력에 의한 이동 계산
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
		// 감속
		AddMoveVector(FVector::Right * _DeltaTime, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);

		// 일정 속도 이하면 멈추기
		if (FinalMoveVector.X > -80.0f)
		{
			MoveVector = FVector::Zero;
			StateChange(EPlayState::Idle);
			return;
		}
	}
	if (MyDir == EActorDir::Right && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// 감속
		AddMoveVector(FVector::Left * _DeltaTime, RunAcc);
		MoveUpdate(_DeltaTime, RunMaxSpeed, RunAcc);

		// 일정 속도 이하면 멈추기
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

// Jump : 점프
void APlayer::Jump(float _DeltaTime)
{
	if (DirCheck())	// 공중에서 방향 전환됐을 경우 남은 속도 초기화
	{
		MoveVector = FVector::Zero;
	}

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		DirCheck();
		PlayerRenderer->ChangeAnimation(GetAnimationName("JumpTurn"));
		AddMoveVector({ static_cast<float>(DirState) * _DeltaTime, 0.f }, MoveAcc); // 점프 중 이동은 Move만 됨
	}

	MoveUpdate(_DeltaTime, JumpMaxSpeed);

	if (abs(FinalMoveVector.Y) < 40.f)
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
		CalMoveVector(_DeltaTime, MaxSpeed);
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

void APlayer::CalMoveVector(float _DeltaTime, float MaxSpeed)
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
	FinalMoveVector += JumpVector;
}

// 최종 계산된 방향과 힘으로 이동시키는 함수
void APlayer::FinalMove(float _DeltaTime) 
{
	FVector MovePos = FinalMoveVector * _DeltaTime;					// 플레이어 이동량 (걷기의 Move가 아님)

	// 플레이어 이동
	AddActorLocation(MovePos);

	// 카메라 이동
	FVector CameraPosLeft = GetWorld()->GetCameraPos();				// 카메라 왼쪽 좌표
	FVector NextCameraPosLeft = CameraPosLeft + MovePos;			// 플레이어 이동 후 왼쪽 카메라 끝 
	FVector NextCameraPosRight = WinScale + NextCameraPosLeft;		// 이동 후의 카메라 오른쪽 끝
	FVector NextPlayerPos = GetActorLocation();						// 플레이어 위치

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

bool APlayer::IsPlayerBottomMagentaA()
{
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 1, Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		return true;
	}
	return false;
}