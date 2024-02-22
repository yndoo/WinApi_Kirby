#include "PlayLevel.h"
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
	ULevel::BeginPlay();
}

void UPlayLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	if (true == Kirby->IsPlayerDoor())
	{
		GEngine->ChangeLevel("RestAreaLevel");
	}
}

void UPlayLevel::LevelStart(ULevel* _Level)
{
	UEngineDirectory NewPath;

	NewPath.MoveParent();

	NewPath.Move("KirbyResources");
	NewPath.Move("PlayLevel");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	// 이 레벨에서 필요한 애니메이션을 위한 이미지들 커팅
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
	UEngineResourcesManager::GetInst().CuttingImage("Flamer_Hurt.png", 2, 1);
	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("Maps\\1_3_foreground"));

	
	AMap* Map = SpawnActor<AMap>();

	Map->SetMapImage("1_3_foreground");
	Map->SetColMapImage("level1-3_foreground01_col.png");
	Map->SetBackMapImage("level1-3_background.png");

	//(background 크기 - 윈도우 창 X크기) / (foreground 크기 - 윈도우 창 X크기)
	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	Map->BackRenderer->SetCameraRatio((1386.f - WinScale.X) / (4720.f - WinScale.X));

	Map->Renderer->CreateAnimation("MapAnimation", "1_3_foreground", 0, 3, 0.5f, true);
	Map->Renderer->ChangeAnimation("MapAnimation");
	

	Kirby = this->SpawnActor<APlayer>();
	this->SpawnActor<AFlamer>();
}
void UPlayLevel::LevelEnd(ULevel* _Level)
{
	// 리소스도 날려요.
	// 액터를 삭제한다.
	int a = 0;
}