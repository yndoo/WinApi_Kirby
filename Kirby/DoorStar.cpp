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
	DoorRenderer = CreateImageRenderer(EKirbyRenderOrder::DoorStar);
	DoorRenderer->SetImage("Door.png");
	DoorRenderer->SetTransform({ {0,30}, {40, 60} });

	StarRenderer = CreateImageRenderer(EKirbyRenderOrder::DoorStar);
	StarRenderer->SetImage("DoorStar.png");
	StarRenderer->CreateAnimation("Twinkle", "DoorStar.png", 0, 1, 0.1f, true);
	StarRenderer->SetTransform({ {0,0}, {300, 300} });

	EndingStarRenderer = CreateImageRenderer(EKirbyRenderOrder::DoorStar);
	EndingStarRenderer->SetImage("BigDoorStar.png");
	EndingStarRenderer->CreateAnimation("EndingStar", "BigDoorStar.png", 0, 3, 0.05f, true);
	EndingStarRenderer->SetTransform({ {0,0}, {300, 300} });

	StarRenderer->ChangeAnimation("Twinkle");
	EndingStarRenderer->ChangeAnimation("EndingStar");
}

void ADoorStar::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}

void ADoorStar::BigStarOn()
{
	StarRenderer->ActiveOff();
	EndingStarRenderer->ActiveOn();
}

void ADoorStar::SmallStarOn()
{
	StarRenderer->ActiveOn();
	EndingStarRenderer->ActiveOff();
}