#include "Spawner.h"
#include "ContentsHelper.h"

ASpawner::ASpawner()
{
}

ASpawner::~ASpawner()
{
}


void ASpawner::BeginPlay()
{
	SpawnCollision = CreateCollision(EKirbyCollisionOrder::Spawner);
	SpawnCollision->SetScale({ 1000,50 });
	SpawnCollision->SetPosition({ 0, 0 });
	SpawnCollision->SetColType(ECollisionType::Rect);
}

void ASpawner::Tick(float _DeltaTime)
{

}