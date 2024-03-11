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
	if (OneFrosty->IsDestroy() && true == Kirby->IsPlayerDoor() && true == UEngineInput::IsDown(VK_UP))
	{
		// 문에 별 생기고 엔딩레벨로 가야함.

		GEngine->ChangeLevel("TitleLevel");	// 임시

	}

	std::vector<UCollision*> Result;
	if (nullptr != FrostySpawner && true == FrostySpawner->SpawnCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		//보스 스폰
		OneFrosty = SpawnActor<AMrFrosty>();
		OneFrosty->SetActorLocation({ 520, 300 });
		OneFrosty->SetMaxHp(200);
		FrostySpawner->SpawnCollision->Destroy();
		FrostySpawner = nullptr;
	}

	std::vector<UCollision*> Result2;
	if (
		nullptr != FrostyStarter && nullptr != OneFrosty &&
		true == FrostyStarter->SpawnCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result2)
		)
	{
		//보스 움직임시작?
		OneFrosty->IsStart = true;
		FrostyStarter->SpawnCollision->Destroy();
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
	this->SpawnActor<APlayer>();

	ALadder* FirstLadder = SpawnActor<ALadder>();
	FirstLadder->SetActorLocation({ 580,1020 });
	FirstLadder->Collision->SetScale({ 40, 199 });

	ALadder* SecondLadder = SpawnActor<ALadder>();
	SecondLadder->SetActorLocation({ 60,800 });
	SecondLadder->Collision->SetScale({ 40, 240 });

	// 1층 나무블록 두 개
	std::vector<AWoodBlock*> WBV1;
	for (int i = 0; i < 2; i++)
	{
		WBV1.push_back(SpawnActor<AWoodBlock>());
		WBV1[i]->SetActorLocation({ 480 + i * 40, 920 });
	}

	// 보스 스폰장치
	FrostySpawner = SpawnActor<ASpawner>();
	FrostySpawner->SetActorLocation({ 0, 400 });

	// 보스 시작장치
	FrostyStarter = SpawnActor<ASpawner>();
	FrostyStarter->SetActorLocation({ 320, 300 });
}
void UBossLevel::LevelEnd(ULevel* _Level)
{

}