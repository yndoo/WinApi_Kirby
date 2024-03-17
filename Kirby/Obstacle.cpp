#include "Obstacle.h"
#include "ContentsHelper.h"

AObstacle::AObstacle()
{
}

AObstacle::~AObstacle()
{
}

void AObstacle::BeginPlay()
{
	AActor::BeginPlay();

	Collision = CreateCollision(EKirbyCollisionOrder::Obstacle);
	Collision->SetScale({ 120, 40 });
	Collision->SetPosition({ 0, 0 });	
	Collision->SetColType(ECollisionType::Rect);
}

void AObstacle::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	std::vector<UCollision*> Result;
	if (false == Touched && true == Collision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		Collision->ActiveOff();
		Touched = true;
		OffTime = 1.5f;
	}

	if (true == Touched)
	{
		OffTime -= _DeltaTime;
		if (OffTime < 0.f)
		{
			Touched = false;
			Collision->ActiveOn();
		}
	}
}