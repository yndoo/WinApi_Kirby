#include "Player.h"
#include<EnginePlatform/EngineInput.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::BeginPlay() {
	AActor::BeginPlay();
	SetActorLocation({ 100,100 });
	
	PlayerRenderer = CreateImageRenderer(0);
	PlayerRenderer->SetImage("DS DSi - Kirby Squeak Squad Kirby Mouse Attack - Kirby.png");

	PlayerRenderer->SetPosition({ 100, 100 });
	PlayerRenderer->SetTransform({ {0,0}, {30, 40} });
	
	// 이미지보다 크게 자르면 아예 안 나옴.
	PlayerRenderer->SetImageCuttingTransform({ {0,0}, {30, 40} });

	PlayerRenderer->CreateAnimation("Test", "DS DSi - Kirby Squeak Squad Kirby Mouse Attack - Kirby.png", 0, 5, 0.5f, true);
	PlayerRenderer->ChangeAnimation("Test");
	

	int a = 0;
	
}

void Player::Tick(float _DeltaTime) {
	if (true == EngineInput::IsPress(VK_LEFT)) {
		AddActorLocation(FVector::Left * 500.0f * _DeltaTime);
	}
	if (true == EngineInput::IsPress(VK_RIGHT)) {
		AddActorLocation(FVector::Right * 500.0f * _DeltaTime);
	}
	if (true == EngineInput::IsPress(VK_UP)) {
		AddActorLocation(FVector::Up * 500.0f * _DeltaTime);
	}
	if (true == EngineInput::IsPress(VK_DOWN)) {
		AddActorLocation(FVector::Down * 500.0f * _DeltaTime);
	}

	if (true == EngineInput::IsPress('X')) {
		// 별 쏘기
	}
}