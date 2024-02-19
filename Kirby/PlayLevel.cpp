#include "PlayLevel.h"
#include "Player.h"
#include "Flamer.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Map.h"

UPlayLevel::UPlayLevel()
{
}

UPlayLevel::~UPlayLevel()
{
}

void UPlayLevel::BeginPlay() {
	ULevel::BeginPlay();

}

void UPlayLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
}

void UPlayLevel::LevelStart(ULevel* _Level)
{
	// ���⼭ ���ҽ��� �ε��ϰ�
	// ���͵� ���⼭ �����
	UEngineDirectory NewPath;

	NewPath.MoveParent();

	NewPath.Move("KirbyResources");
	NewPath.Move("Level1-3");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	// �� �������� �ʿ��� �ִϸ��̼��� ���� �̹����� Ŀ��
	UEngineResourcesManager::GetInst().CuttingImage("Kirby.png", 40, 13);
	UEngineResourcesManager::GetInst().CuttingImage("Move_Right.png", 10, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Move_Left.png", 10, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Idle_Right.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Idle_Left.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Crouch_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Crouch_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Slide_Right.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Slide_Left.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Run_Right.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Run_Left.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Jump_Right.png", 9, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Jump_Left.png", 9, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Break_Right.png", 1, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Break_Left.png", 1, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Flamer_Spin.png", 4, 1);
	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("Maps\\foreground"));

	AMap* Map = SpawnActor<AMap>();
	
	Map->SetMapImage("foreground");
	Map->SetColMapImage("level1-3_foreground01_col.png");
	Map->SetBackMapImage("level1-3_background.png");
	
	this->SpawnActor<APlayer>();
	this->SpawnActor<AFlamer>();
}
void UPlayLevel::LevelEnd(ULevel* _Level)
{
	// ���ҽ��� ������.
	// ���͸� �����Ѵ�.
	int a = 0;
}