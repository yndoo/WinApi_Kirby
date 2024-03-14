#include "KirbyCore.h"
#include "TitleLevel.h"
#include "PlayLevel.h"
#include "RestAreaLevel.h"
#include "BossLevel.h"
#include "EndingLevel.h"
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

void KirbyCore::BeginPlay() 
{
	float MyX = 2.5f;
	float MyY = 2.5f;

	MainWindow.SetWindowScale({ 255 * MyX, 208 * MyY });

	// 커비는 어디서나 등장하므로 커비코어에서 이미지 로드
	UEngineDirectory NewPath;
	NewPath.MoveParent();
	NewPath.Move("KirbyResources");
	NewPath.Move("CoreResources");
	std::list<UEngineFile> AllFileList_Core = NewPath.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFileList_Core)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	//Effects
	UEngineResourcesManager::GetInst().CuttingImage("DoorStar.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("BigDoorStar.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("EndingStartEffects.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MonDieEffects.png", 12, 3);
	//Bullets
	UEngineResourcesManager::GetInst().CuttingImage("StarBulletIdle_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("StarBulletIdle_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("FireBulletIdle_Right.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("FireBulletIdle_Left.png", 7, 1);
	// Monsters
	UEngineResourcesManager::GetInst().CuttingImage("Flamer_Spin.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Flamer_Damaged.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WaddleDee_Right.png", 5, 2);
	UEngineResourcesManager::GetInst().CuttingImage("WaddleDee_Left.png", 5, 2); 
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyIdle_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyIdle_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyMove_Right.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyMove_Left.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_ShootReady.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyShoot_Right.png", 6, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MrFrostyShoot_Left.png", 6, 1);

	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedIdle_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedIdle_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedMove_Right.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedMove_Left.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedShootReady.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedShoot_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedShoot_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedHitWall_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MF_DamagedHitWall_Left.png", 2, 1);
	//UI
	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("UI\\MonHpAnimation"));

	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UPlayLevel>("PlayLevel");
	CreateLevel<URestAreaLevel>("RestAreaLevel");
	CreateLevel<UBossLevel>("BossLevel");
	CreateLevel<UEndingLevel>("EndingLevel");

	ChangeLevel("PlayLevel");
	//ChangeLevel("TitleLevel");	// 잠시 빼둠
}

void KirbyCore::Tick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown(VK_F2))
	{
		GEngine->EngineDebugSwitch();
	}

	// 테스트용 레벨 전환
	if (true == UEngineInput::IsDown(VK_F3))
	{
		GEngine->ChangeLevel("BossLevel");
	}
	if (true == UEngineInput::IsDown(VK_F4))
	{
		GEngine->ChangeLevel("EndingLevel");
	}
}

void KirbyCore::End() 
{
	int a = 0;
}
