#include "Map.h"
#include "ContentsHelper.h"
#include <EngineCore/EngineResourcesManager.h>

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


void AMap::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
}

void AMap::BeginPlay() {
	Renderer = CreateImageRenderer(KirbyRenderOrder::Map);
	ColRenderer = CreateImageRenderer(KirbyRenderOrder::Map);
	ColRenderer->SetActive(false);
}