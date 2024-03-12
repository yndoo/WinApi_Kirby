#include "EffectHelper.h"
#include "ContentsHelper.h"

EffectHelper::EffectHelper()
{
}

EffectHelper::~EffectHelper()
{
}

void EffectHelper::BeginPlay()
{
	EffectRenderer1 = CreateImageRenderer(EKirbyRenderOrder::Effect);
	EffectRenderer1->SetImage("EndingStartEffects.png");
	EffectRenderer1->SetTransform({ {0,-20}, {400, 400} });

	EffectRenderer1->CreateAnimation("TwinkleEffect", "EndingStartEffects.png", 2, 4, 0.1f, false);
	EffectRenderer1->CreateAnimation("CircleEffect", "EndingStartEffects.png", 5, 7, 0.1f, false);

	EffectRenderer1->ChangeAnimation("CircleEffect");
}
void EffectHelper::Tick(float _DeltaTime)
{

}