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
	MainWindow.SetWindowScale({ 800, 600 });

	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UPlayLevel>("PlayLevel");

	ChangeLevel("TitleLevel");
}

void KirbyCore::Tick(float _DeltaTime) {
	int a = 0;
}

void KirbyCore::End() {
	int a = 0;
}
