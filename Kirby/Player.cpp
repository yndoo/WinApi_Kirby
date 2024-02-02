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
	PlayerRenderer->SetImage("Kirby.png");

	PlayerRenderer->SetPosition({ 100, 100 });
	PlayerRenderer->SetTransform({ {0,0}, {300, 300} });
	PlayerRenderer->SetTransColor(Color8Bit::Blue);
	
	
	// 이미지보다 크게 자르면 아예 안 나옴.
	//PlayerRenderer->SetImageCuttingTransform({ {0,0}, {60, 80} });

	PlayerRenderer->CreateAnimation("Test", "Kirby.png", 11, 19, 0.2f, true);
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