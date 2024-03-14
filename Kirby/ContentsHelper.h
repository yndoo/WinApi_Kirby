#pragma once
#include <EngineCore\ImageRenderer.h>
enum class EKirbyRenderOrder
{
	Map,
	DoorStar,
	Monster,
	Player,
	Bullet,
	Effect,
	UI,
};

enum class EKirbyCollisionOrder
{
	InhaleCol,		// 흡입 충돌체
	MonsterAround,
	Monster,
	Boss,
	PlayerBullet,	// 공격 충돌체
	EdibleBullet,
	Player,
	Ladder,
	Block,
	Spawner,
};

enum class EActorDir
{
	None = 0,
	Left = -1,
	Right = 1,
};

enum class EKirbyState
{
	None,
	CameraFreeMove,
	FreeMove,
	Idle,
	Move,
	Crouch,
	Slide,
	Run,
	Jump,
	Brake,
	Inhale,
	Eating,
	EatingRun,
	Swallow,
	Fly,
	Attack,
	LadderUp,
	LadderDown,
	Exhale,
	Damaged,
	InhaleFail,
};

enum class EKirbyType
{
	None,
	Normal,
	Fire,
};

enum class EEnemyState
{
	None,
	Idle,
	Damaged,
	Move,
	Inhaled,
	Die,
	HitWall,
	ShootReady,
	ShootJump,
	Shoot,
};

enum class EBulletState
{
	None,
	Idle,
	Move,
	Damage,
	Finish,
	Inhaled, // EdibleBullet은 Inhaled가 필요함
};

// 설명 :
class UContentsHelper
{
public:
	static UWindowImage* ColMapImage;
	static UWindowImage* BackgroundImage;
	static bool EatingFireMonster;			// 입 안에 Fire속성 몬스터가 있는 지(아직 삼키지X)
	static EKirbyType KirbyType;
protected:

private:
	// constrcuter destructer
	UContentsHelper();
	~UContentsHelper();

};

