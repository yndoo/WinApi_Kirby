#include "Player.h"
#include<EnginePlatform/EngineInput.h>
#include "ContentsHelper.h"

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

	PlayerRenderer->SetPosition({ 100, 100 });
	PlayerRenderer->SetTransform({ {0,0}, {300, 300} });
	PlayerRenderer->SetTransColor(Color8Bit::Blue);
	
	
	// 이미지보다 크게 자르면 아예 안 나옴.
	//PlayerRenderer->SetImageCuttingTransform({ {0,0}, {60, 80} });

	PlayerRenderer->CreateAnimation("Move_Right", "Kirby.png", 11, 19, 0.1f, true);
	PlayerRenderer->CreateAnimation("Idle_Right", "Kirby.png", 0, 0, 0.1f, true);
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
		//이동
		Move(_DeltaTime);
		break;
	default:
		break;
	}
}

// Idle : 가만히 있는 상태
void  APlayer::Idle(float _DeltaTime) {
	if (
		true == EngineInput::IsPress(VK_LEFT) ||
		true == EngineInput::IsPress(VK_RIGHT)
		)
	{
		StateChange(EPlayState::Move);
		return;
	}

	GravityCheck(_DeltaTime);
	PlayerRenderer->ChangeAnimation(GetAnimationName("Idle"));
}

// Move : 맵 이동(오른쪽 왼쪽)
void APlayer::Move(float _DeltaTime) {
	GravityCheck(_DeltaTime);

	FVector MovePos = FVector::Zero;
	if (EngineInput::IsPress(VK_LEFT))
	{
		MovePos += FVector::Left * _DeltaTime * MoveSpeed;
	}

	if (EngineInput::IsPress(VK_RIGHT))
	{
		MovePos += FVector::Right * _DeltaTime * MoveSpeed;
	}

	if (true == EngineInput::IsFree(VK_LEFT) && EngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);	// Move하다가 가만히 있는 경우
		return;
	}

	AddActorLocation(MovePos);
	PlayerRenderer->ChangeAnimation(GetAnimationName("Move")); 
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
	if (EngineInput::IsPress(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (EngineInput::IsPress(VK_RIGHT))
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