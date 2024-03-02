#include "KirbyCore.h"
#include "TitleLevel.h"
#include "PlayLevel.h"
#include "RestAreaLevel.h"
#include "BossLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

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

	// Ŀ��� ��𼭳� �����ϹǷ� Ŀ���ھ�� �̹��� �ε�
	UEngineDirectory NewPath;
	NewPath.MoveParent();
	NewPath.Move("KirbyResources");
	NewPath.Move("Kirby");
	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UPlayLevel>("PlayLevel");
	CreateLevel<URestAreaLevel>("RestAreaLevel");
	CreateLevel<UBossLevel>("BossLevel");

	ChangeLevel("PlayLevel");
	//ChangeLevel("TitleLevel");	// ��� ����
}

void KirbyCore::Tick(float _DeltaTime) {
	if (true == UEngineInput::IsDown(VK_F2))
	{
		GEngine->EngineDebugSwitch();
	}
}

void KirbyCore::End() {
	int a = 0;
}
