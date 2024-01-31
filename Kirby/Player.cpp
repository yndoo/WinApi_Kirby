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
	
	PlayerRenderer = CreateImageRenderer(0);
	PlayerRenderer->SetPosition({ 0, 30 });
	PlayerRenderer->SetImageToScale("KirbyTest.png");
	
	SetActorLocation({ 100,100 });
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
		// º° ½î±â
	}
}