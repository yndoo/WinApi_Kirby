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

	Map = SpawnActor<AMap>();
	Map->Renderer->CreateAnimation("RestAreaMapAnimation", "restarea_foreground", 0, 3, 0.5f, true);

	LifeItem = SpawnActor<AItem>(EKirbyRenderOrder::Item);
	LifeItem->SetLifeItem();
	LifeItem->SetActorLocation({ 320, 160 });
}

void URestAreaLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	// Ŀ�� Hp ��ȭ ������ UI Update
	if (KirbyHpData != Kirby->GetCurHp())
	{
		KirbyHpData = Kirby->GetCurHp();
		UI->SetKirbyHpUI(KirbyHpData);
	}
	// Ŀ�� Life �� ��ȭ ������ UI Update
	if (KirbyLifeData != Kirby->GetKirbyLife())
	{
		KirbyLifeData = Kirby->GetKirbyLife();
		UI->SetKirbyLifeUI(KirbyLifeData);
	}

	if (true == Kirby->IsPlayerDoor() && true == UEngineInput::IsDown(VK_UP))
	{
		GEngine->ChangeLevel("PlayLevel");
	}
}

void URestAreaLevel::LevelStart(ULevel* _Level)
{
	Map->SetMapImage("restarea_foreground");
	Map->SetColMapImage("restarea_colmap.png");
	Map->SetBackMapImage("RestAreaBackground.png");

	Map->Renderer->ChangeAnimation("RestAreaMapAnimation");

	this->SpawnActor<APlayer>();
	
	UI = SpawnActor<KirbyUI>();
	KirbyHpData = Kirby->GetCurHp();
	UI->SetKirbyHpUI(KirbyHpData);
	KirbyLifeData = Kirby->GetKirbyLife();
	UI->SetKirbyLifeUI(KirbyLifeData);

}
void URestAreaLevel::LevelEnd(ULevel* _Level)
{
	UI->Destroy();
}