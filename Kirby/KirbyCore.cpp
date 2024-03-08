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

	// 커비는 어디서나 등장하므로 커비코어에서 이미지 로드
	UEngineDirectory NewPath;
	NewPath.MoveParent();
	NewPath.Move("KirbyResources");
	NewPath.Move("Kirby");
	std::list<UEngineFile> AllFileList_K = NewPath.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFileList_K)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}
	NewPath.MoveParent();
	NewPath.Move("Monster");
	std::list<UEngineFile> AllFileList_M = NewPath.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFileList_M)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	UEngineResourcesManager::GetInst().CuttingImage("StarBulletIdle_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("StarBulletIdle_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("FireBulletIdle_Right.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("FireBulletIdle_Left.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Flamer_Spin.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Flamer_Damaged.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WaddleDee_Right.png", 5, 2);
	UEngineResourcesManager::GetInst().CuttingImage("WaddleDee_Left.png", 5, 2); 
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyIdle_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyIdle_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyMove_Right.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyMove_Left.png", 3, 1);

	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UPlayLevel>("PlayLevel");
	CreateLevel<URestAreaLevel>("RestAreaLevel");
	CreateLevel<UBossLevel>("BossLevel");

	ChangeLevel("BossLevel");
	//ChangeLevel("TitleLevel");	// 잠시 빼둠
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
