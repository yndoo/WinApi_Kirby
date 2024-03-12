#include "EndingKirby.h"
#include "ContentsHelper.h"

AEndingKirby::AEndingKirby()
{
}

AEndingKirby::~AEndingKirby()
{
}

void AEndingKirby::BeginPlay()
{
	EffectHelper::BeginPlay();

	Renderer = CreateImageRenderer(EKirbyRenderOrder::Player);
	Renderer->SetImage("HappyKirby.png");
	Renderer->SetTransform({ {0,-20}, {50, 52} });

	EffectRenderer1->ChangeAnimation("CircleEffect");
}
void AEndingKirby::Tick(float _DeltaTime)
{
	if (EffectRenderer1->GetCurAnimation()->Name == UEngineString::ToUpper("CircleEffect") && true == EffectRenderer1->IsCurAnimationEnd())
	{
		EffectRenderer1->ChangeAnimation("TwinkleEffect");
	}
	if (EffectRenderer1->GetCurAnimation()->Name == UEngineString::ToUpper("TwinkleEffect") && true == EffectRenderer1->IsCurAnimationEnd())
	{
		EffectRenderer1->ActiveOff();
	}
}