#include "TitleLogo.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>

TitleLogo::TitleLogo()
{
}

TitleLogo::~TitleLogo()
{
}

void TitleLogo::BeginPlay() {
	AActor::BeginPlay();

}

void TitleLogo::Tick(float _DeltaTime) {
	if (true == EngineInput::IsPress('A')) {
		GEngine->ChangeLevel("PlayLevel");
	}
}
