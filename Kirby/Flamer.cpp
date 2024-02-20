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

	FlamerRenderer = CreateImageRenderer(0);
	FlamerRenderer->SetImage("Flamer_Spin.png");
	FlamerRenderer->SetTransform({ {0,0}, {220, 220} });
	FlamerRenderer->SetTransColor(Color8Bit::Magenta);

	FlamerRenderer->CreateAnimation("Flamer_Spin", "Flamer_Spin.png", 0, 3, 0.2f, true);
	FlamerRenderer->ChangeAnimation("Flamer_Spin");

	// 일단 잔디블록에 떨어지게 함...
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color != Color8Bit(255, 0, 255, 0))
		{
			AddActorLocation(FVector::Down);
		}
		else
		{
			break;
		}
	}

	//ActorScale = GetTransform().GetScale();
	StateChange(EEnemyState::Idle);
}

void AFlamer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void AFlamer::StateChange(EEnemyState _State)
{
	switch (State)
	{
	case EEnemyState::None:
		break;
	case EEnemyState::Idle:
		IdleStart();
		break;
	default:
		break;
	}
	State = _State;
}

void AFlamer::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EEnemyState::None:
		break;
	case EEnemyState::Idle:
		Idle(_DeltaTime);
		break;
	default:
		break;
	}
}


void AFlamer::IdleStart()
{
	FlamerRenderer->ChangeAnimation("Flamer_Spin");
}

void AFlamer::Idle(float _DeltaTime)
{
	// 마젠타에 붙어있는지 확인하기 위해 붙어있는 방향 확인
	int i = CurDir;
	i = (i + 1) % 4;

	int X = GetActorLocation().iX() + dx[i];
	int Y = GetActorLocation().iY() + dy[i];
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(X, Y, Color8Bit::MagentaA);

	if (Color != Color8Bit(255, 0, 255, 0))
	{
		// 마젠타에 닿아있지 않는 경우 탐색
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
			if (Color == Color8Bit(255, 0, 255, 0))
			{
				// 방향이 바뀌는 순간
				AddActorLocation({ dx[i] * 0.17f, dy[i] * 0.17f });
				CurDir = i;
				break;
			}
		}
	}
	else
	{
		// 마젠타에 닿아있을 경우
		// 현재 방향대로 이동
		AddActorLocation({ dx[CurDir] * 0.17f, dy[CurDir] * 0.17f });
	}
}
