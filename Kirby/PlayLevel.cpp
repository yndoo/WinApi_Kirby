#include "PlayLevel.h"
#include "Player.h"
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
	// 여기서 리소스를 로드하고
	// 액터도 여기서 만들고
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

	// 이 레벨에서 필요한 애니메이션을 위한 이미지들
	UEngineResourcesManager::GetInst().CuttingImage("Kirby.png", 40, 13);
	UEngineResourcesManager::GetInst().CuttingImage("Move_Right.png", 10, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Move_Left.png", 10, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Idle_Right.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Idle_Left.png", 3, 1);

	AMap* Map = SpawnActor<AMap>();
	Map->SetMapImage("level1-3_foreground01.png");
	Map->SetColMapImage("level1-3_foreground01_col.png");
	Map->SetBackMapImage("level1-3_background.png");
	
	this->SpawnActor<APlayer>();
}
void UPlayLevel::LevelEnd(ULevel* _Level)
{
	// 리소스도 날려요.
	// 액터를 삭제한다.
	int a = 0;
}