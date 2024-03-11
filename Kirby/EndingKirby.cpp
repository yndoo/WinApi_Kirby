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
	Renderer = CreateImageRenderer(EKirbyRenderOrder::Player);
	Renderer->SetImage("HappyKirby.png");
	Renderer->SetTransform({ {0,-20}, {50, 52} });

	EffectRenderer1 = CreateImageRenderer(EKirbyRenderOrder::Effect);
	EffectRenderer1->SetImage("HappyKirby.png");
	EffectRenderer1->SetTransform({ {0,-20}, {300, 300} });

	EffectRenderer1->CreateAnimation("TwinkleEffect", "EndingStartEffects.png", 2, 4, 0.1f, false);
	EffectRenderer1->CreateAnimation("CircleEffect", "EndingStartEffects.png", 5, 7, 0.1f, false);

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