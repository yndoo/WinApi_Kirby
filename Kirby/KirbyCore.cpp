#include "KirbyCore.h"
#include "TitleLevel.h"
#include "PlayLevel.h"

KirbyCore::KirbyCore()
	: UEngineCore()
{
}

KirbyCore::~KirbyCore()
{
}

void KirbyCore::BeginPlay() {
	MainWindow.SetWindowScale({ 1280, 720 });

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
