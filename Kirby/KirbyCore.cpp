#include "KirbyCore.h"
#include "TitleLevel.h"
#include "PlayLevel.h"

KirbyCore::KirbyCore()
	: EngineCore()
{
}

KirbyCore::~KirbyCore()
{
}

void KirbyCore::BeginPlay() {
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UPlayLevel>("PlayLevel");

	ChangeLevel("PlayLevel");
}

void KirbyCore::Tick(float _DeltaTime) {
	int a = 0;
}

void KirbyCore::End() {
	int a = 0;
}
