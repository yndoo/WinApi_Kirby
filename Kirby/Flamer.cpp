#include "Flamer.h"

AFlamer::AFlamer()
{
}

AFlamer::~AFlamer()
{
}

void AFlamer::BeginPlay() {
	AActor::BeginPlay();

	SetActorLocation({ 100,100 });

	FlamerRenderer = CreateImageRenderer(0);
	FlamerRenderer->SetImage("Flamer_Spin.png");
	FlamerRenderer->SetTransform({ {0,0}, {300, 300} });
	FlamerRenderer->SetTransColor(Color8Bit::Magenta);

	FlamerRenderer->CreateAnimation("Flamer_Spin", "Flamer_Spin.png", 0, 3, 0.1f, true);
	FlamerRenderer->ChangeAnimation("Flamer_Spin");
}

void AFlamer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
}