#include "DoorStar.h"
#include "ContentsHelper.h"

ADoorStar::ADoorStar()
{
}

ADoorStar::~ADoorStar()
{
}

void ADoorStar::BeginPlay()
{
	Renderer = CreateImageRenderer(EKirbyRenderOrder::DoorStar);
	Renderer->SetImage("DoorStar.png");
	Renderer->CreateAnimation("Twinkle", "DoorStar.png", 0, 1, 0.1f, true);
	Renderer->SetTransform({ {0,0}, {300, 300} });

	Renderer->ChangeAnimation("Twinkle");
}
void ADoorStar::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}