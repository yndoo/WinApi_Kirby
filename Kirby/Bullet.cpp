#include "Bullet.h"

ABullet::ABullet()
{
}

ABullet::~ABullet()
{
}

void ABullet::BeginPlay()
{
	BulletRenderer = CreateImageRenderer(EKirbyRenderOrder::Bullet);
	BulletRenderer->SetImage("Bullet.png");
	BulletRenderer->SetTransform({ {0,0}, {40, 40} });
	BulletRenderer->SetTransColor(Color8Bit::Magenta);

	BulletRenderer->CreateAnimation("Bullet", "Bullet.png", 0, 1, 0.2f, true);
	BulletRenderer->ChangeAnimation("Bullet");
}

void ABullet::Tick(float _DeltaTime)
{

}