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

void UPlayLevel::CuttingLR(std::string_view _Name, int _X, int _Y)
{
	UEngineResourcesManager::GetInst().CuttingImage(std::string(_Name) + "_Left.png", _X, _Y);
	UEngineResourcesManager::GetInst().CuttingImage(std::string(_Name) + "_Right.png", _X, _Y);
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
	CuttingLR("Move", 10, 1);
	CuttingLR("Idle", 3, 1);
	CuttingLR("Crouch", 2, 1);
	CuttingLR("Slide", 2, 1);
	CuttingLR("Run", 8, 1);
	CuttingLR("Jump", 9, 1);
	CuttingLR("Inhale", 13, 1);
	CuttingLR("Eating", 7, 1);
	CuttingLR("EatingMove", 15, 1);
	CuttingLR("EatingJump", 9, 1);
	CuttingLR("Swallow", 5, 1);

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
		// �� ��ġ�� ���� �ٸ� ������ �̵��ϵ���.
		if (Kirby->BeforePos.X > 3350 && Kirby->BeforePos.X < 3400)
		{
			GEngine->ChangeLevel("RestAreaLevel");
		}
		else if (Kirby->BeforePos.X > 4550)
		{
			GEngine->ChangeLevel("BossLevel");
		}
	}
}

void UPlayLevel::LevelStart(ULevel* _Level)
{
	Map->SetMapImage("1_3_foreground");
	Map->SetColMapImage("level1-3_foreground01_col.png");
	Map->SetBackMapImage("level1-3_background.png");

	//(background ũ�� - ������ â Xũ��) / (foreground ũ�� - ������ â Xũ��)
	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	Map->BackRenderer->SetCameraRatio((1386.f - WinScale.X) / (4720.f - WinScale.X));
	
	Map->Renderer->ChangeAnimation("MapAnimation");

	this->SpawnActor<APlayer>();
	AFlamer* Flamer1 = this->SpawnActor<AFlamer>();
	Flamer1->SetActorLocation({ 500,250 });
	Flamer1->StartColor = Color8Bit::YellowA;
	//AFlamer* Flamer2 = this->SpawnActor<AFlamer>();
	//Flamer2->SetActorLocation({ 1000,300 });
	//Flamer2->StartColor = Color8Bit::MagentaA;
}

void UPlayLevel::LevelEnd(ULevel* _Level)
{
	
}