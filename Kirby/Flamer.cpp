#include "Flamer.h"
#include <EngineBase/EngineDebug.h>

AFlamer::AFlamer()
{
}

AFlamer::~AFlamer()
{
}

void AFlamer::BeginPlay() {
	AActor::BeginPlay();

	SetActorLocation({ 500,250 });
	SetMaxHp(50);
	SetName("Flamer");

	MonsterRenderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	MonsterRenderer->SetImage("Flamer_Spin.png");
	MonsterRenderer->SetTransform({ {0,0}, {220, 220} });
	MonsterRenderer->SetTransColor(Color8Bit::Magenta);

	MonsterRenderer->CreateAnimation("Spin", "Flamer_Spin.png", 0, 3, 0.2f, true);
	MonsterRenderer->CreateAnimation("Damaged", "Flamer_Damaged.png", { 0,0,0,0,1 }, 0.2f, false);
	MonsterRenderer->CreateAnimation("DieEffect", "MonDieEffects.png", 0, 23, 0.05f, false);
	MonsterRenderer->ChangeAnimation("Spin");

	MonsterCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	MonsterCollision->SetScale({ 40, 40 });
	MonsterCollision->SetColType(ECollisionType::Rect);

	MapSize = UContentsHelper::ColMapImage->GetScale();

	FallDown(MoveColor);
	StateChange(EEnemyState::Idle);
}

void AFlamer::Tick(float _DeltaTime) 
{
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	// 커비 흡입 충돌체와 몬스터의 충돌 확인
	std::vector<UCollision*> Result;
	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::InhaleCol, Result))
	{
		// 커비쪽으로 당겨지기
		InhaleDir = Result[0]->GetOwner()->GetActorLocation() - GetActorLocation();
		AddActorLocation(InhaleDir.Normalize2DReturn() * 100.f * _DeltaTime);
		StateChange(EEnemyState::Inhaled);
		return;
	}

	if (State != EEnemyState::Inhaled && false == DeathCheck && nullptr != MonsterCollision && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		StateChange(EEnemyState::Damaged);
		return;
	}

	if (false == DeathCheck && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::PlayerBullet, Result))
	{
		// Bullet종류로 공격 받았을 때
		StateChange(EEnemyState::Damaged);
		return;
	}
}


void AFlamer::IdleStart()
{
	FVector ActorPos = GetActorLocation();
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(ActorPos.iX(), ActorPos.iY(), Color8Bit::MagentaA);
	if (Color != MoveColor)
	{
		FallDown(MoveColor);
	}
	MonsterRenderer->ChangeAnimation("Spin");
}
void AFlamer::Idle(float _DeltaTime)
{
	if (LateStart == true)
	{
		// 나중에 Start함수를 돌려줘야하는 객체의 경우 FallDown을 다시 해줌.
		FallDown(MoveColor);
	}
	ColorLineMove(_DeltaTime, MoveColor);
}

void AFlamer::DamagedStart()
{
	IsDamaged = true;
	AddDamageHp(60);
	MonsterRenderer->ChangeAnimation("Damaged");
}
void AFlamer::Damaged(float _DeltaTime)
{
	// 나중에 체력 넣으면 체력이 0될 때 죽어야 함(미완)
	if (GetCurHp() <= 0)
	{
		StateChange(EEnemyState::Die);
		return;
	}

	// 바닥에 떨어진 후 Move
	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		FallDown(Color8Bit::MagentaA);
		StateChange(EEnemyState::Move);
		return;
	}

}

void AFlamer::DieStart()
{
	IsDamaged = false;
	//Die 애니메이션 실행
	MonsterRenderer->ChangeAnimation("DieEffect");
}
void AFlamer::Die(float _DeltaTime)
{
	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		MonsterRenderer->ActiveOff();
		MonsterCollision->ActiveOff();
		//Destroy();
	}
}

void AFlamer::MoveStart()
{
	MonsterRenderer->ChangeAnimation("Spin");
}
void AFlamer::Move(float _DeltaTime)
{
	CurDir = 0;
	ColorLineMove(_DeltaTime, Color8Bit::MagentaA);

	// 맵 넘어가면 없애기
	FVector MyPos = GetActorLocation();
	if (MyPos.X < -10 || MyPos.X > MapSize.X)
	{
		Destroy();
	}
}

void AFlamer::InhaledStart()
{
	MonsterRenderer->ChangeAnimation("Damaged");
}
void AFlamer::Inhaled(float _DeltaTime)
{
	// 흡입되다가 커비 몸과 충돌됐을 경우
	std::vector<UCollision*> Result;
	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		UContentsHelper::EatingFireMonster = true;
		Destroy();
	}
}

void AFlamer::ColorLineMove(float _DeltaTime, Color8Bit _Color)
{
	// _Color에 붙어있는지 확인하기 위해 붙어있는 방향 확인
	int i = CurDir;
	i = (i + 1) % 4;

	FVector Pos = GetActorLocation();
	int X = Pos.iX() + dx[i];
	int Y = Pos.iY() + dy[i];
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(X, Y, Color8Bit::MagentaA);

	if (Color != _Color)
	{
		// _Color색에 닿아있지 않는 경우 탐색
		int i = CurDir;
		while (true)
		{
			// 한 칸 이전 위치로 돌려놓고
			int nX = X + dx[(i + 2) % 4];
			int nY = Y + dy[(i + 2) % 4];

			// 반시계방향으로 돌려서 확인
			i = (i + 1) % 4;
			nX += dx[i];
			nY += dy[i];
			Color = UContentsHelper::ColMapImage->GetColor(nX, nY, Color8Bit::MagentaA);
			if (Color == _Color)
			{
				// 방향이 바뀌는 순간
				AddActorLocation({ dx[i] * MoveSpeed * _DeltaTime , dy[i] * MoveSpeed * _DeltaTime });
				CurDir = i;
				break;
			}
		}
	}
	else
	{
		// _Color에 닿아있을 경우
		// 현재 방향대로 이동
		AddActorLocation({ dx[CurDir] * MoveSpeed * _DeltaTime, dy[CurDir] * MoveSpeed * _DeltaTime });
	}
}

