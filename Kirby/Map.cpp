#include "Map.h"
#include "ContentsHelper.h"
#include <EngineCore/EngineResourcesManager.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>

AMap::AMap()
{
}

AMap::~AMap()
{
}

void AMap::SetMapImage(std::string_view _MapImageName)
{
	Renderer->SetImage(_MapImageName);
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void AMap::SetColMapImage(std::string_view _MapImageName)
{
	ColRenderer->SetImage(_MapImageName);
	UWindowImage* Image = ColRenderer->GetImage();
	UContentsHelper::ColMapImage = Image;
	FVector ImageScale = Image->GetScale();
	ColRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void AMap::SetBackMapImage(std::string_view _MapImageName) {
	BackRenderer->SetImage(_MapImageName);
	UWindowImage* Image = BackRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	BackRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void AMap::SwitchDebug()
{
	if (true == Renderer->IsActive())
	{
		Renderer->ActiveOff();
		ColRenderer->ActiveOn();
	}
	else
	{
		Renderer->ActiveOn();
		ColRenderer->ActiveOff();
	}

}

void AMap::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	if (UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}
}

void AMap::BeginPlay() {
	BackRenderer = CreateImageRenderer(EKirbyRenderOrder::Map);
	Renderer = CreateImageRenderer(EKirbyRenderOrder::Map);
	ColRenderer = CreateImageRenderer(EKirbyRenderOrder::Map);
	ColRenderer->ActiveOff();
}