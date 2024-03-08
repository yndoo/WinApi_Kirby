#include "MrFrosty.h"
#include "ContentsHelper.h"

AMrFrosty::AMrFrosty()
{
}

AMrFrosty::~AMrFrosty()
{
}

void AMrFrosty::BeginPlay()
{
	Renderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	Renderer->SetImage("MrFrostyIdle_Left.png");
	Renderer->SetTransform({ {0,0}, {300, 300} });

	BodyCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	BodyCollision->SetScale({ 40, 40 });
	BodyCollision->SetPosition({ 0, -20 });
	BodyCollision->SetColType(ECollisionType::Rect);

	Renderer->CreateAnimation("Idle_Right", "MrFrostyIdle_Right.png", { 0,0,0,1 }, 0.5f, true);
	Renderer->CreateAnimation("Idle_Left", "MrFrostyIdle_Left.png", { 0,0,0,1 }, 0.5f, true);

	Renderer->ChangeAnimation("Idle_Left");
}

void AMrFrosty::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void AMrFrosty::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EEnemyState::Idle:
		Idle(_DeltaTime);
		break;
	}
}

void AMrFrosty::StateChange(EEnemyState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EEnemyState::Idle:
			IdleStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void AMrFrosty::IdleStart()
{
	DirCheck();
	Renderer->ChangeAnimation(GetAnimationName("Idle"));
}

void AMrFrosty::Idle(float _DeltaTime)
{

}
