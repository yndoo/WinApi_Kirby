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

void UPlayLevel::CuttingImgLR(std::string_view _Name, int _X, int _Y)
{
	UEngineResourcesManager::GetInst().CuttingImage(std::string(_Name) + "_Left.png", _X, _Y);
	UEngineResourcesManager::GetInst().CuttingImage(std::string(_Name) + "_Right.png", _X, _Y);
}

void UPlayLevel::BeginPlay() {
	ULevel::BeginPlay();

	// 이 레벨에서 필요한 이미지들 커팅 및 로딩 : 커비(로딩은 커비코어에서 함), 몬스터, 불렛, 맵
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

	//UEngineResourcesManager::GetInst().CuttingImage("Kirby.png", 40, 13);
	CuttingImgLR("Move", 10, 1);
	CuttingImgLR("Idle", 3, 1);
	CuttingImgLR("Crouch", 2, 1);
	CuttingImgLR("Slide", 2, 1);
	CuttingImgLR("Run", 8, 1);
	CuttingImgLR("Jump", 9, 1);
	CuttingImgLR("Inhale", 13, 1);
	CuttingImgLR("Eating", 7, 1);
	CuttingImgLR("EatingMove", 15, 1);
	CuttingImgLR("EatingJump", 9, 1);
	CuttingImgLR("Swallow", 5, 1);
	CuttingImgLR("EatingAttack", 5, 1);
	CuttingImgLR("FireIdle", 8, 1);
	CuttingImgLR("FireSlide", 5, 1);
	CuttingImgLR("FireMove", 10, 2);
	CuttingImgLR("FireRun", 8, 1);
	CuttingImgLR("FireBreak", 2, 1);
	CuttingImgLR("FireCrouch", 8, 1);
	CuttingImgLR("FireJump", 11, 1);
	CuttingImgLR("FireAttack", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("LadderMove.png", 13, 1);
	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("Maps\\1_3_foreground"));

	
	Map = SpawnActor<AMap>();

	Map->Renderer->CreateAnimation("MapAnimation", "1_3_foreground", 0, 3, 0.5f, true);
}

void UPlayLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (true == Kirby->IsPlayerDoor() && true == UEngineInput::IsDown(VK_UP))
	{
		// 레벨 넘어가기 전에 위치를 저장해둬야함.
		Kirby->BeforePos = Kirby->GetTransform().GetPosition();
		// 문 위치에 따라 다른 레벨로 이동하도록.
		if (Kirby->BeforePos.X > 3350 && Kirby->BeforePos.X < 3400)
		{
			GEngine->ChangeLevel("RestAreaLevel");
		}
		else if (Kirby->BeforePos.X > 4550)
		{
			GEngine->ChangeLevel("BossLevel");
		}
	}

	// 테스트용 레벨 전환
	if (true == UEngineInput::IsDown(VK_F3))
	{
		GEngine->ChangeLevel("BossLevel");
	}
}

void UPlayLevel::LevelStart(ULevel* _Level)
{
	Map->SetMapImage("1_3_foreground");
	Map->SetColMapImage("level1-3_foreground01_col.png");
	Map->SetBackMapImage("level1-3_background.png");

	//(background 크기 - 윈도우 창 X크기) / (foreground 크기 - 윈도우 창 X크기)
	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	Map->BackRenderer->SetCameraRatio((1386.f - WinScale.X) / (4720.f - WinScale.X));
	
	Map->Renderer->ChangeAnimation("MapAnimation");

	this->SpawnActor<APlayer>();

	AFlamer* Flamer1 = this->SpawnActor<AFlamer>();
	Flamer1->SetActorLocation({ 500,250 });
	Flamer1->MoveColor = Color8Bit::YellowA;

	AFlamer* Flamer2 = this->SpawnActor<AFlamer>();
	Flamer2->SetActorLocation({ 1000,300 });
	Flamer2->MoveColor = Color8Bit::MagentaA;
	Flamer2->LateStart = true;

	AWaddleDee* WaddleDee1 = SpawnActor<AWaddleDee>();
	AWaddleDee* WaddleDee2 = SpawnActor<AWaddleDee>();
	WaddleDee2->SetActorLocation({ 2000, 200 });
}

void UPlayLevel::LevelEnd(ULevel* _Level)
{
	
}