#include "KirbyCore.h"
#include "TitleLevel.h"

KirbyCore::KirbyCore()
	: EngineCore()
{
}

KirbyCore::~KirbyCore()
{
}

void KirbyCore::BeginPlay() {
	CreateLevel<UTitleLevel>("TitleLevel");

	ChangeLevel("TitleLevel");
}

void KirbyCore::Tick(float _DeltaTime) {
	int a = 0;
}

void KirbyCore::End() {
	int a = 0;
}
