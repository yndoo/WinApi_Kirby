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

	Map = SpawnActor<AMap>(EKirbyRenderOrder::Map);

	// 사다리
	for (int i = 0; i < 2; i++)
	{
		Ladders[i] = SpawnActor<ALadder>();
	}
	Ladders[0]->SetActorLocation({ 580,1020 });
	Ladders[0]->Collision->SetScale({ 40, 199 });

	Ladders[1]->SetActorLocation({ 60,800 });
	Ladders[1]->Collision->SetScale({ 40, 240 });

	// 아이템
	TypeItem = SpawnActor<AItem>(EKirbyRenderOrder::Item);
	TypeItem->SetSpinCenter({ 184, 513 });
	TypeItem->SetFireTypeItem();
	TypeItem->SetActorLocation({ 184, 513 });
}
void UBossLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

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
	// 보스(Mr.Frosty) Hp 변화 있으면 UI Update
	if (nullptr != OneFrosty && FrostyHpData != OneFrosty->GetCurHp())
	{
		FrostyHpData = OneFrosty->GetCurHp();
		UI->SetBossHpUI(OneFrosty->GetMaxHp(), FrostyHpData);
	}

	//FVector CamPos = GetCameraPos();
	//UEngineDebug::DebugTextPrint("X : " + std::to_string(CamPos.X) + ", Y : " + std::to_string(CamPos.Y), 30.0f);
	if (nullptr != OneFrosty && true == OneFrosty->DeathCheck)
	{
		// 문에 별 생기고 엔딩레벨로 가야함.
		if (false == OpenDoorOnce)
		{
			ADoorStar* DStar = SpawnActor<ADoorStar>();
			DStar->SetActorLocation({ 540, 261 });
			DStar->BigStarOn();
			OpenDoorOnce = true;
		}
		
		if (true == Kirby->IsPlayerDoor() && true == UEngineInput::IsDown(VK_UP))
		{
			GEngine->ChangeLevel("EndingLevel");	
		}
	}

	std::vector<UCollision*> Result;
	if (nullptr != FrostySpawner && true == FrostySpawner->SpawnCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		//보스 스폰
		OneFrosty = SpawnActor<AMrFrosty>();
		OneFrosty->SetActorLocation({ 520, 300 });
		OneFrosty->SetMaxHp(200);
		UI->BossUIOn();
		FrostyHpData = OneFrosty->GetCurHp();
		FrostySpawner->SpawnCollision->ActiveOff();
	}

	std::vector<UCollision*> Result2;
	if (
		nullptr != FrostyStarter && nullptr != OneFrosty &&
		true == FrostyStarter->SpawnCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result2)
		)
	{
		//보스 움직임시작?
		OneFrosty->IsStart = true;
		FrostyStarter->SpawnCollision->ActiveOff();
	}

	//테스트용
	if (true == UEngineInput::IsDown(VK_SPACE))
	{
		Kirby->AddActorLocation({ 150, -500 });
		AddCameraPos({ 0, -500 });
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
	APlayer* kb = this->SpawnActor<APlayer>(EKirbyRenderOrder::Player);
	UContentsHelper::EatingFireMonster = Kirby->GetEatingFireType();

	UI = SpawnActor<KirbyUI>();
	KirbyHpData = Kirby->GetCurHp();
	UI->SetKirbyHpUI(KirbyHpData);
	KirbyLifeData = Kirby->GetKirbyLife();
	UI->SetKirbyLifeUI(KirbyLifeData);

	//// 1층 나무블록 두 개
	//std::vector<AWoodBlock*> WBV1;
	//for (int i = 0; i < 2; i++)
	//{
	//	WBV1.push_back(SpawnActor<AWoodBlock>());
	//	WBV1[i]->SetActorLocation({ 480 + i * 40, 920 });
	//}

	// 보스 스폰장치
	FrostySpawner = SpawnActor<ASpawner>();
	FrostySpawner->SetActorLocation({ 0, 400 });

	// 보스 시작장치
	FrostyStarter = SpawnActor<ASpawner>();
	FrostyStarter->SetActorLocation({ 320, 300 });

	// 몬스터
	for (int i = 0; i < 2; i++)
	{
		WDees[i] = SpawnActor<AWaddleDee>(EKirbyRenderOrder::Monster);
	}
	WDees[0]->SetActorLocation({ 485, 1116 });
	WDees[1]->SetActorLocation({405, 910});
}
void UBossLevel::LevelEnd(ULevel* _Level)
{
	UI->Destroy();
	if (OneFrosty != nullptr)
	{
		OneFrosty->Destroy();
	}
	FrostySpawner->Destroy();
	FrostyStarter->Destroy();
	for (int i = 0; i < 2; i++)
	{
		WDees[i]->Destroy();
	}
}