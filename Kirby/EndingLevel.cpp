#include "EndingLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "EndingKirby.h"
#include <EngineCore/EngineCore.h>
#include "Player.h"
#include "ContentsHelper.h"

UEndingLevel::UEndingLevel()
{
}

UEndingLevel::~UEndingLevel()
{
}

void UEndingLevel::BeginPlay()
{
	ULevel::BeginPlay();

	UEngineDirectory NewPath;

	NewPath.MoveParent();

	NewPath.Move("KirbyResources");
	NewPath.Move("EndingLevel");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	UEngineResourcesManager::GetInst().LoadFolder(NewPath.AppendPath("DanceCut"));

	Map = SpawnActor<AMap>();
	Map->BackRenderer->CreateAnimation("Dance", "DanceCut", 0, 204, 0.033f, false);
}

void UEndingLevel::Tick(float _DeltaTime)
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

	if (true == Map->BackRenderer->IsCurAnimationEnd())
	{
		//³¡
		GEngine->ChangeLevel("TitleLevel");
	}

	Timer -= _DeltaTime;
	if (Timer <= 0.f)
	{
		EndingSound.SetVolume(0.1f);
		EndingSound = UEngineSound::SoundPlay("Clap.wav");
	}
}

void UEndingLevel::LevelStart(ULevel* _Level)
{
	UI = SpawnActor<KirbyUI>();
	UI->SetKirbyHpUI(Kirby->GetCurHp());
	UI->SetKirbyLifeUI(Kirby->GetKirbyLife());

	Map->SetMapImage("ending_foreground_map.png");
	Map->SetColMapImage("ending_background_colmap.png");
	Map->SetBackMapImage("ending_background_map.png");
	Map->ColRenderer->ActiveOff();

	Map->BackRenderer->ChangeAnimation("Dance");

	UContentsHelper::EndingLevelStart = true;

	bgm = UEngineSound::SoundPlay("EndingBgm.wav");
}
void UEndingLevel::LevelEnd(ULevel* _Level)
{
	
}