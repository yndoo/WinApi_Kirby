#include "KirbyCore.h"
#include "TitleLevel.h"
#include "PlayLevel.h"
#include "RestAreaLevel.h"

KirbyCore::KirbyCore()
	: UEngineCore()
{
}

KirbyCore::~KirbyCore()
{
}

void KirbyCore::BeginPlay() {
	float MyX = 2.5f;
	float MyY = 2.5f;

	MainWindow.SetWindowScale({ 255 * MyX, 208 * MyY });

	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UPlayLevel>("PlayLevel");
	CreateLevel<URestAreaLevel>("RestAreaLevel");

	ChangeLevel("PlayLevel");
}

void KirbyCore::Tick(float _DeltaTime) {
	int a = 0;
}

void KirbyCore::End() {
	int a = 0;
}
