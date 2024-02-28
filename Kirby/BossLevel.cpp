#include "BossLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineDebug.h>

UBossLevel::UBossLevel()
{
}

UBossLevel::~UBossLevel()
{
}

void UBossLevel::BeginPlay()
{
	UEngineDirectory NewPath;

	NewPath.MoveParent();

	NewPath.Move("KirbyResources");
	NewPath.Move("BossLevel");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	Map = SpawnActor<AMap>();
}
void UBossLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	//FVector CamPos = GetCameraPos();
	//UEngineDebug::DebugTextPrint("X : " + std::to_string(CamPos.X) + ", Y : " + std::to_string(CamPos.Y), 30.0f);
	if (true == Kirby->IsPlayerDoor() && true == UEngineInput::IsDown(VK_UP))
	{
		GEngine->ChangeLevel("PlayLevel");
	}
}
void UBossLevel::LevelStart(ULevel* _Level)
{
	Map->SetMapImage("bosslevel_map.png");
	Map->SetColMapImage("bosslevel_colmap.png");
	Map->BackRenderer->ActiveOff();
	
	MapSize = Map->Renderer->GetTransform().GetScale();
	WinScale = GEngine->MainWindow.GetWindowScale();

	SetCameraPos({ 0.f, MapSize.Y - WinScale.Y});
	this->SpawnActor<APlayer>();
}
void UBossLevel::LevelEnd(ULevel* _Level)
{

}