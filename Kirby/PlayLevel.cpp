#include "PlayLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "RestAreaLevel.h"
#include <EngineCore/EngineCore.h>

UPlayLevel::UPlayLevel()
{
}

UPlayLevel::~UPlayLevel()
{
}

void UPlayLevel::BeginPlay() {
	ULevel::BeginPlay();
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
	UEngineResourcesManager::GetInst().CuttingImage("Flamer_Hurt.png", 2, 1);
	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("Maps\\1_3_foreground"));

	Map = SpawnActor<AMap>();

	Map->Renderer->CreateAnimation("MapAnimation", "1_3_foreground", 0, 3, 0.5f, true);
}

void UPlayLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (true == Kirby->IsPlayerDoor() && true == UEngineInput::IsDown(VK_UP))
	{
		// ���� �Ѿ�� ���� ��ġ�� �����ص־���.
		Kirby->BeforePos = Kirby->GetTransform().GetPosition();
		GEngine->ChangeLevel("RestAreaLevel");
	}
}

void UPlayLevel::LevelStart(ULevel* _Level)
{
	Map->SetMapImage("1_3_foreground");
	Map->SetColMapImage("level1-3_foreground01_col_magenta.png");
	Map->SetBackMapImage("level1-3_background.png");

	//(background ũ�� - ������ â Xũ��) / (foreground ũ�� - ������ â Xũ��)
	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	Map->BackRenderer->SetCameraRatio((1386.f - WinScale.X) / (4720.f - WinScale.X));
	
	Map->Renderer->ChangeAnimation("MapAnimation");

	this->SpawnActor<APlayer>();
	this->SpawnActor<AFlamer>();

}

void UPlayLevel::LevelEnd(ULevel* _Level)
{
	
}