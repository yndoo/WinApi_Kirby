#include "Flamer.h"

AFlamer::AFlamer()
{
}

AFlamer::~AFlamer()
{
}

void AFlamer::BeginPlay() {
	AActor::BeginPlay();

	SetActorLocation({ 500,250 });

	FlamerRenderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	FlamerRenderer->SetImage("Flamer_Spin.png");
	FlamerRenderer->SetTransform({ {0,0}, {220, 220} });
	FlamerRenderer->SetTransColor(Color8Bit::Magenta);

	FlamerRenderer->CreateAnimation("Spin", "Flamer_Spin.png", 0, 3, 0.2f, true);
	FlamerRenderer->CreateAnimation("Hurt", "Flamer_Hurt.png", { 0,0,0,0,1 }, 0.2f, false);
	FlamerRenderer->ChangeAnimation("Spin");

	// 일단 잔디블록에 떨어지게 함...
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color != StartColor)
		{
			AddActorLocation(FVector::Down);
		}
		else
		{
			break;
		}
	}
	StateChange(EEnemyState::Idle);

	FlamerCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	FlamerCollision->SetScale({ 40, 40 });
	FlamerCollision->SetColType(ECollisionType::Rect);

	MapSize = UContentsHelper::ColMapImage->GetScale();
}

void AFlamer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	// 커비 흡입 충돌체와 몬스터의 충돌 확인
	std::vector<UCollision*> Result;
	if (true == FlamerCollision->CollisionCheck(EKirbyCollisionOrder::PlayerBullet, Result))
	{
		// 커비쪽으로 당겨지기
		InhaleDir = Result[0]->GetOwner()->GetActorLocation() - GetActorLocation();
		FVector test = InhaleDir.Normalize2DReturn();
		AddActorLocation(InhaleDir.Normalize2DReturn() * 80.f * _DeltaTime);
		StateChange(EEnemyState::Inhaled);
		return;
	}
}

void AFlamer::StateChange(EEnemyState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EEnemyState::Idle:
			IdleStart();
			break;
		case EEnemyState::Hurt:
			HurtStart();
			break;
		case EEnemyState::Move:
			MoveStart();
			break;
		case EEnemyState::Inhaled:
			InhaledStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void AFlamer::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EEnemyState::Idle:
		Idle(_DeltaTime);
		break;
	case EEnemyState::Hurt:
		Hurt(_DeltaTime);
		break;
	case EEnemyState::Move:
		Move(_DeltaTime);
		break;
	case EEnemyState::Inhaled:
		Inhaled(_DeltaTime);
		break;
	default:
		break;
	}
}

void AFlamer::IdleStart()
{
	FlamerRenderer->ChangeAnimation("Spin");
}

void AFlamer::HurtStart()
{
	FlamerRenderer->ChangeAnimation("Hurt");
}

void AFlamer::MoveStart()
{
	FlamerRenderer->ChangeAnimation("Spin");
}

void AFlamer::InhaledStart()
{
	FlamerRenderer->ChangeAnimation("Hurt");
}

void AFlamer::Idle(float _DeltaTime)
{
	ColorMove(_DeltaTime, StartColor);

	std::vector<UCollision*> Result;
	if (nullptr != FlamerCollision && true == FlamerCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		StateChange(EEnemyState::Hurt);
		return;
	}
}

void AFlamer::Move(float _DeltaTime)
{
	CurDir = 0;
	ColorMove(_DeltaTime, Color8Bit::MagentaA);

	// 맵 넘어가면 없애기
	FVector MyPos = GetActorLocation();
	if (MyPos.X < -10 || MyPos.X > MapSize.X)
	{
		Destroy();
	}
}

void AFlamer::Hurt(float _DeltaTime)
{
	// 나중에 체력 넣으면 체력이 0될 때 죽어야 함(미완)

	// 바닥에 떨어진 후 Move
	FVector MyPos = GetActorLocation();
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(MyPos.iX(), MyPos.iY(), Color8Bit::MagentaA);
	if (Color != Color8Bit::MagentaA)
	{
		AddActorLocation(FVector::Down * 100.0f * _DeltaTime);
	}
	else
	{
		StateChange(EEnemyState::Move);
		return;
	}
}

void AFlamer::Inhaled(float _DeltaTime)
{
	// 흡입되다가 커비 몸과 충돌됐을 경우
	std::vector<UCollision*> Result;
	if (true == FlamerCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		Destroy();
	}
}

void AFlamer::ColorMove(float _DeltaTime, Color8Bit _Color)
{
	// _Color에 붙어있는지 확인하기 위해 붙어있는 방향 확인
	int i = CurDir;
	i = (i + 1) % 4;

	int X = GetActorLocation().iX() + dx[i];
	int Y = GetActorLocation().iY() + dy[i];
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
				AddActorLocation({ dx[i] * MoveSpeed , dy[i] * MoveSpeed });
				CurDir = i;
				break;
			}
		}
	}
	else
	{
		// _Color에 닿아있을 경우
		// 현재 방향대로 이동
		AddActorLocation({ dx[CurDir] * MoveSpeed , dy[CurDir] * MoveSpeed });
	}
}