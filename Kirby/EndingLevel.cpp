#include "EndingLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "EndingKirby.h"

UEndingLevel::UEndingLevel()
{
}

UEndingLevel::~UEndingLevel()
{
}

void UEndingLevel::BeginPlay()
{
	ULevel::BeginPlay();
	UEngineDirectory NewPath;

	NewPath.MoveParent();

	NewPath.Move("KirbyResources");
	NewPath.Move("EndingLevel");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("DanceCut"));

	Map = SpawnActor<AMap>();
	Map->BackRenderer->CreateAnimation("Dance", "DanceCut", 0, 204, 0.03f, false);
}

void UEndingLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

}

void UEndingLevel::LevelStart(ULevel* _Level)
{
	Map->SetMapImage("ending_foreground_map.png");
	Map->SetColMapImage("ending_background_colmap.png");
	Map->SetBackMapImage("ending_background_map.png");
	Map->ColRenderer->ActiveOff();
	//Map->Renderer->ActiveOff();

	Map->BackRenderer->ChangeAnimation("Dance");

	//AEndingKirby* Kb = SpawnActor<AEndingKirby>();
	//Kb->SetActorLocation({ 317, 338 });
}
void UEndingLevel::LevelEnd(ULevel* _Level)
{
	
}