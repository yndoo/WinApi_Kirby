#include "TitleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EngineCore/EngineCore.h>
#include "TitleLogo.h"

UTitleLevel::UTitleLevel()
{
}

UTitleLevel::~UTitleLevel()
{
}

void UTitleLevel::BeginPlay() {
	ULevel::BeginPlay();

	UEngineDirectory NewPath;

	NewPath.MoveParent();

	NewPath.Move("KirbyResources");
	NewPath.Move("Title");

	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	// 타이틀로고
	AActor* TitleActor = SpawnActor<TitleLogo>();
	Renderer = TitleActor->CreateImageRenderer();
	FVector WindowScale = GEngine->MainWindow.GetWindowScale();


	Renderer->SetImage("Title.png");
	Renderer->SetScale(WindowScale.Half2D());
	Renderer->SetPosition(WindowScale.Half2D());

}

void UTitleLevel::Tick(float _DeltaTime) {
	ULevel::Tick(_DeltaTime);

	AlphaTime += _DeltaTime;
	if (1.0f <= AlphaTime)
	{
		Dir = !Dir;
		AlphaTime = 0.0f;
	}

	if (true == Dir)
	{
		Renderer->SetAlpha(AlphaTime);
	}
	else
	{
		Renderer->SetAlpha(1.0f - AlphaTime);
	}
}

void UTitleLevel::LevelStart(ULevel* _Level)
{
	// 여기서 리소스를 로드하고
	// 액터도 여기서 만들고
	int a = 0;
}
void UTitleLevel::LevelEnd(ULevel* _Level)
{
	// 리소스도 날려요.
	// 액터를 삭제한다.
	int a = 0;
}