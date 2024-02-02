#include "PlayLevel.h"
#include "Player.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

UPlayLevel::UPlayLevel()
{
}

UPlayLevel::~UPlayLevel()
{
}

void UPlayLevel::BeginPlay() {
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

	UEngineResourcesManager::GetInst().CuttingImage("Kirby.png", 40, 13);

	this->SpawnActor<Player>();
}
