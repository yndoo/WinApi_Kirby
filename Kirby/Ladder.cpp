#include "Ladder.h"
#include "ContentsHelper.h"

ALadder::ALadder()
{
}

ALadder::~ALadder()
{
}

void ALadder::BeginPlay()
{
	AActor::BeginPlay();

	Collision = CreateCollision(EKirbyCollisionOrder::Ladder);
	Collision->SetScale({ 40, 200 });
	Collision->SetPosition({ 0, 0 });
	Collision->SetColType(ECollisionType::Rect);
}

void ALadder::Tick(float _DeltaTime)
{

}