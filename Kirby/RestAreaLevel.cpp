#include "RestAreaLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

URestAreaLevel::URestAreaLevel()
{
}

URestAreaLevel::~URestAreaLevel()
{
}

void URestAreaLevel::BeginPlay() 
{
	ULevel::BeginPlay();

	UEngineDirectory NewPath;

	NewPath.MoveParent();

	NewPath.Move("KirbyResources");
	NewPath.Move("RestAreaLevel");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("Maps\\restarea_foreground"));

	{
		AMap* Map = SpawnActor<AMap>();

		Map->SetMapImage("restarea_foreground");
		Map->SetColMapImage("restarea_colmap.png");
		Map->SetBackMapImage("RestAreaBackground.png");

		//(background 크기 - 윈도우 창 X크기) / (foreground 크기 - 윈도우 창 X크기)
		FVector WinScale = GEngine->MainWindow.GetWindowScale();
		Map->BackRenderer->SetCameraRatio((1386.f - WinScale.X) / (4720.f - WinScale.X));

		Map->Renderer->CreateAnimation("RestAreaMapAnimation", "restarea_foreground", 0, 3, 0.5f, true);
		Map->Renderer->ChangeAnimation("RestAreaMapAnimation");
	}
}

void URestAreaLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
}

void URestAreaLevel::LevelStart(ULevel* _Level)
{

}
void URestAreaLevel::LevelEnd(ULevel* _Level)
{

}