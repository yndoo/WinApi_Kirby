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
	PlayerRenderer->SetImageToScale("DS DSi - Kirby Squeak Squad Kirby Mouse Attack - Kirby.png");
	
	SetActorLocation({ 100,100 });
	SetActorScale({ 100,100 });
}

void Player::Tick(float _DeltaTime) {
	if (true == EngineInput::IsPress('A')) {
		AddActorLocation(FVector::Left * 500.0f * _DeltaTime);
	}
	if (true == EngineInput::IsPress('D')) {
		AddActorLocation(FVector::Right * 500.0f * _DeltaTime);
	}
}