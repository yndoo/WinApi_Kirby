#include "Flamer.h"

AFlamer::AFlamer()
{
}

AFlamer::~AFlamer()
{
}

void AFlamer::BeginPlay() {
	AActor::BeginPlay();

	SetActorLocation({ 500,250 });

	FlamerRenderer = CreateImageRenderer(0);
	FlamerRenderer->SetImage("Flamer_Spin.png");
	FlamerRenderer->SetTransform({ {0,0}, {250, 250} });
	FlamerRenderer->SetTransColor(Color8Bit::Magenta);

	FlamerRenderer->CreateAnimation("Flamer_Spin", "Flamer_Spin.png", 0, 3, 0.5f, true);
	FlamerRenderer->ChangeAnimation("Flamer_Spin");

	StateChange(EEnemyState::Idle);
}

void AFlamer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void AFlamer::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EEnemyState::None:
		break;
	case EEnemyState::Idle:
		break;
	default:
		break;
	}
}

void AFlamer::StateChange(EEnemyState _State)
{
	switch (State)
	{
	case EEnemyState::None:
		break;
	case EEnemyState::Idle:
		break;
	default:
		break;
	}
	State = _State;
}

void AFlamer::Idle(float _DeltaTime)
{
	// 블록을 돌고있음.. ㄱ-
	// 마젠타를 따라서 걸어다니는 걸로 하면 될듯!!
}

void AFlamer::IdleStart()
{

}