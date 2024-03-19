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

	//NewPath.MoveParent();
	//NewPath.Move("KirbyResources");
	//NewPath.Move("PlayLevel");

	NewPath.MoveToSearchChild("KirbyResources");
	NewPath.MoveToSearchChild("PlayLevel");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	//UEngineResourcesManager::GetInst().CuttingImage("Kirby.png", 40, 13);
	CuttingImgLR("Idle", 3, 1);
	CuttingImgLR("Move", 10, 1);
	CuttingImgLR("Crouch", 2, 1);
	CuttingImgLR("Slide", 2, 1);
	CuttingImgLR("Run", 8, 1);
	CuttingImgLR("Jump", 9, 1);
	CuttingImgLR("Inhale", 13, 1);
	CuttingImgLR("Fly", 10, 1);
	CuttingImgLR("Damaged", 9, 1);
	CuttingImgLR("Swallow", 5, 1);

	CuttingImgLR("Eating", 7, 1);
	CuttingImgLR("EatingMove", 15, 1);
	CuttingImgLR("EatingJump", 9, 1);
	CuttingImgLR("EatingAttack", 5, 1);
	
	CuttingImgLR("FireIdle", 8, 1);
	CuttingImgLR("FireSlide", 5, 1);
	CuttingImgLR("FireMove", 10, 2);
	CuttingImgLR("FireRun", 8, 1);
	CuttingImgLR("FireBrake", 2, 1);
	CuttingImgLR("FireCrouch", 8, 1);
	CuttingImgLR("FireJump", 11, 1);
	CuttingImgLR("FireAttack", 4, 1);
	CuttingImgLR("FireFlyStart", 5, 1);
	CuttingImgLR("FireFlying", 5, 3);
	CuttingImgLR("FireExhale", 3, 1);

	CuttingImgLR("IceIdle", 2, 1);
	CuttingImgLR("IceMove", 10, 1);
	CuttingImgLR("IceCrouch", 2, 1);
	CuttingImgLR("IceSlide", 2, 1);
	CuttingImgLR("IceRun", 8, 1);
	CuttingImgLR("IceJump", 10, 1);
	CuttingImgLR("IceAttack", 8, 1);
	CuttingImgLR("IceFlyStart", 5, 1);
	CuttingImgLR("IceFlying", 8, 1);
	CuttingImgLR("IceExhale", 2, 1);

	UEngineResourcesManager::GetInst().CuttingImage("LadderMove.png", 13, 1);
	UEngineResourcesManager::GetInst().CuttingImage("FireLadderUp.png", 4, 10);
	UEngineResourcesManager::GetInst().CuttingImage("FireLadderDown.png", 4, 4);
	UEngineResourcesManager::GetInst().CuttingImage("IceLadderUp.png", 10, 1);
	UEngineResourcesManager::GetInst().CuttingImage("IceLadderDown.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("KirbyDance.png", 10, 3);
	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("Maps\\1_3_foreground"));

	Map = SpawnActor<AMap>(EKirbyRenderOrder::Map);
	Map->Renderer->CreateAnimation("MapAnimation", "1_3_foreground", 0, 3, 0.5f, true);

	// 아이템
	/*IceItem = SpawnActor<AItem>(EKirbyRenderOrder::Item);
	IceItem->SetSpinCenter({ 1680,240 });
	IceItem->SetIceTypeItem();*/

	// 문
	restDoor = SpawnActor<ADoorStar>(EKirbyRenderOrder::DoorStar);
	restDoor->SetActorLocation({ 3380, 107 });
	restDoor->SmallStarOn();
	
	bossDoor = SpawnActor<ADoorStar>(EKirbyRenderOrder::DoorStar);
	bossDoor->SetActorLocation({ 4580, 223 });
	bossDoor->SmallStarOn();
}

void UPlayLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown(VK_TAB))
	{
		if (true == UI->ManualRenderer->IsActive())
		{
			UI->ManualRenderer->ActiveOff();
		}
		else
		{
			UI->ManualRenderer->ActiveOn();
		}
	}

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

	// 커비 Hp 변화 있으면 UI Update
	if (KirbyHpData != Kirby->GetCurHp())
	{
		KirbyHpData = Kirby->GetCurHp();
		UI->SetKirbyHpUI(KirbyHpData);
	}
	// 커비 Life 수 변화 있으면 UI Update
	if (KirbyLifeData != Kirby->GetKirbyLife())
	{
		KirbyLifeData = Kirby->GetKirbyLife();
		UI->SetKirbyLifeUI(KirbyLifeData);
	}
	// 몬스터 각각 변화 있으면 UI Update
	// Flamer
	for (int i = 0; i < 2; i++)
	{
		if (FlamerHpData[i] != Flamers[i]->GetCurHp())
		{
			FlamerHpData[i] = Flamers[i]->GetCurHp();
			UI->MonsterAniOn();
		}
	}
	// WaddleDee
	for (int i = 0; i < 2; i++)
	{
		if (WDeeHpData[i] != WDees[i]->GetCurHp())
		{
			WDeeHpData[i] = WDees[i]->GetCurHp();
			UI->MonsterAniOn();
		}
	}
}

void UPlayLevel::LevelStart(ULevel* _Level)
{
	//bgm = UEngineSound::SoundPlay("PlayLevelBGM.wav");

	Map->SetMapImage("1_3_foreground");
	Map->SetColMapImage("level1-3_foreground01_col.png");
	Map->SetBackMapImage("level1-3_background.png");

	//(background 크기 - 윈도우 창 X크기) / (foreground 크기 - 윈도우 창 X크기)
	FVector WinScale = GEngine->MainWindow.GetWindowScale();
	Map->BackRenderer->SetCameraRatio((1386.f - WinScale.X) / (4720.f - WinScale.X));
	Map->Renderer->ChangeAnimation("MapAnimation");

	if (true == UContentsHelper::CameraRestart)
	{
		SetCameraPos({ 0,0 });
		UContentsHelper::CameraRestart = false;
	}

	// Kirby
	this->SpawnActor<APlayer>(EKirbyRenderOrder::Player);

	// Flamer
	for (int i = 0; i < 2; i++)
	{
		if(nullptr == Flamers[i])
		{
			Flamers[i] = SpawnActor<AFlamer>();
			FlamerHpData[i] = Flamers[i]->GetMaxHp();
		}
	}
	Flamers[0]->SetActorLocation({500,250});
	Flamers[0]->MoveColor = Color8Bit::YellowA;

	Flamers[1]->SetActorLocation({ 1500,300 });
	Flamers[1]->MoveColor = Color8Bit::MagentaA;
	Flamers[1]->LateStart = true;

	// WaddleDee
	for (int i = 0; i < 4; i++)
	{
		if (nullptr == WDees[i])
		{
			WDees[i] = SpawnActor<AWaddleDee>();
			WDees[i]->SetActorLocation({ (i + 1) * 1000, 200 });
			WDeeHpData[i] = WDees[i]->GetMaxHp();
		}
	}

	UI = SpawnActor<KirbyUI>(EKirbyRenderOrder::UI);
	KirbyHpData = Kirby->GetCurHp();
	UI->SetKirbyHpUI(KirbyHpData);
	KirbyLifeData = Kirby->GetKirbyLife();
	UI->SetKirbyLifeUI(KirbyLifeData);
}

void UPlayLevel::LevelEnd(ULevel* _Level)
{
	UI->Destroy();
	for (int i = 0; i < 2; i++)
	{
		if (false == Flamers[i]->IsDestroy()) 
		{
			Flamers[i]->Destroy();
		}
		Flamers[i] = nullptr;
	}
	for (int i = 0; i < 2; i++)
	{
		if (false == WDees[i]->IsDestroy())
		{
			WDees[i]->Destroy();
		}
		WDees[i] = nullptr;
	}
}