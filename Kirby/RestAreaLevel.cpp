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

	//NewPath.MoveParent();

	//NewPath.Move("KirbyResources");
	//NewPath.Move("RestAreaLevel");

	NewPath.MoveToSearchChild("KirbyResources");
	NewPath.MoveToSearchChild("RestAreaLevel");

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
	LifeItem->SetActorLocation({ 320, 340 });

	IceItem = SpawnActor<AItem>(EKirbyRenderOrder::Item);
	IceItem->SetIceTypeItem();
	IceItem->SetSpinCenter({ 320, 130 });
	IceItem->SetActorLocation({ 320, 130 });
}

void URestAreaLevel::Tick(float _DeltaTime)
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

	this->SpawnActor<APlayer>(EKirbyRenderOrder::Player);
	
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