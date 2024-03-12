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
	Renderer->SetTransform({ {0,0}, {300, 300} });
	//Renderer->CreateAnimation("Dance", "DanceCut", 0, 231, 0.01f, false);
	//Renderer->ChangeAnimation("Dance");

	//EffectRenderer1 = CreateImageRenderer(EKirbyRenderOrder::Effect);
	//EffectRenderer1->SetImage("EndingStartEffects.png");
	//EffectRenderer1->SetTransform({ {0,-20}, {400, 400} });

	//// CreateAnimation
	//Renderer->CreateAnimation("Dance", "KirbyDance.png", 0, 29, 0.2f, false);

	//EffectRenderer1->CreateAnimation("TwinkleEffect", "EndingStartEffects.png", 2, 4, 0.1f, false);
	//EffectRenderer1->CreateAnimation("CircleEffect", "EndingStartEffects.png", 5, 7, 0.1f, false);

	//EffectRenderer1->ChangeAnimation("CircleEffect");
}
void AEndingKirby::Tick(float _DeltaTime)
{
	// ÀÌÆåÆ® : CircleÀÌÆåÆ® -> TwinkleÀÌÆåÆ®
	// Ä¿ºñ : »ý¼º ÈÄ 1~2ÃÊ ÈÄ Ãã ½ÃÀÛ...
	//if (EffectRenderer1->GetCurAnimation()->Name == UEngineString::ToUpper("CircleEffect") && true == EffectRenderer1->IsCurAnimationEnd())
	//{
	//	EffectRenderer1->ChangeAnimation("TwinkleEffect");
	//}
	//if (EffectRenderer1->GetCurAnimation()->Name == UEngineString::ToUpper("TwinkleEffect") && true == EffectRenderer1->IsCurAnimationEnd())
	//{
	//	EffectRenderer1->ActiveOff();
	//}
	//if (false == EffectRenderer1->IsActive())
	//{
	//	Renderer->ChangeAnimation("Dance");
	//}
}