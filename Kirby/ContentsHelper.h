#pragma once
#include <EngineCore\ImageRenderer.h>
enum class EKirbyRenderOrder
{
	Map,
	DoorStar,
	Item,
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
	IcedMonster,
	Boss,
	PlayerBullet,	// 공격 충돌체
	IceBreathBullet,
	EdibleBullet,
	LifeItem,
	IceTypeItem,
	FireTypeItem,
	Player,
	Ladder,
	Block,
	Spawner,
	Obstacle,
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
	Changing,
};

enum class EKirbyType
{
	None,
	Normal,
	Eating,
	Fire,
	Ice,
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
	Iced,
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
	static EKirbyType KirbyType;
	static bool EatingFireMonster;			// 입 안에 Fire속성 몬스터가 있는 지(아직 삼키지X)
	static bool CameraRestart;				// LevelRestart시에 카메라도 리스타트 해야하는지의 여부 (true면 카메라 위치 리셋 필요)
	static bool EndingLevelStart;
protected:

private:
	// constrcuter destructer
	UContentsHelper();
	~UContentsHelper();

};

