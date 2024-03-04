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

	FlamerRenderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	FlamerRenderer->SetImage("Flamer_Spin.png");
	FlamerRenderer->SetTransform({ {0,0}, {220, 220} });
	FlamerRenderer->SetTransColor(Color8Bit::Magenta);

	FlamerRenderer->CreateAnimation("Spin", "Flamer_Spin.png", 0, 3, 0.2f, true);
	FlamerRenderer->CreateAnimation("Hurt", "Flamer_Hurt.png", { 0,0,0,0,1 }, 0.2f, false);
	FlamerRenderer->ChangeAnimation("Spin");

	FlamerCollision = CreateCollision(EKirbyCollisionOrder::Monster);
	FlamerCollision->SetScale({ 40, 40 });
	FlamerCollision->SetColType(ECollisionType::Rect);

	MapSize = UContentsHelper::ColMapImage->GetScale();

	FallDown(MoveColor);
	StateChange(EEnemyState::Idle);

}

void AFlamer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);

	// Ŀ�� ���� �浹ü�� ������ �浹 Ȯ��
	std::vector<UCollision*> Result;
	if (true == FlamerCollision->CollisionCheck(EKirbyCollisionOrder::InhaleCol, Result))
	{
		// Ŀ�������� �������
		InhaleDir = Result[0]->GetOwner()->GetActorLocation() - GetActorLocation();
		FVector test = InhaleDir.Normalize2DReturn();
		AddActorLocation(InhaleDir.Normalize2DReturn() * 80.f * _DeltaTime);
		StateChange(EEnemyState::Inhaled);
		return;
	}

	if (true == FlamerCollision->CollisionCheck(EKirbyCollisionOrder::PlayerBullet, Result))
	{
		// Bullet������ ���� �޾��� ��
		StateChange(EEnemyState::Hurt);
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
	FVector ActorPos = GetActorLocation();
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(ActorPos.iX(), ActorPos.iY(), Color8Bit::MagentaA);
	if (Color != MoveColor)
	{
		FallDown(MoveColor);
	}
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
	if (LateStart == true)
	{
		// ���߿� Start�Լ��� ��������ϴ� ��ü�� ��� FallDown�� �ٽ� ����.
		FallDown(MoveColor);
	}
	ColorLineMove(_DeltaTime, MoveColor);

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
	ColorLineMove(_DeltaTime, Color8Bit::MagentaA);

	// �� �Ѿ�� ���ֱ�
	FVector MyPos = GetActorLocation();
	if (MyPos.X < -10 || MyPos.X > MapSize.X)
	{
		Destroy();
	}
}

void AFlamer::Hurt(float _DeltaTime)
{
	// ���߿� ü�� ������ ü���� 0�� �� �׾�� ��(�̿�)

	// �ٴڿ� ������ �� Move
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
	// ���ԵǴٰ� Ŀ�� ���� �浹���� ���
	std::vector<UCollision*> Result;
	if (true == FlamerCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		Destroy();
	}
}

void AFlamer::ColorLineMove(float _DeltaTime, Color8Bit _Color)
{
	// _Color�� �پ��ִ��� Ȯ���ϱ� ���� �پ��ִ� ���� Ȯ��
	int i = CurDir;
	i = (i + 1) % 4;

	FVector Pos = GetActorLocation();
	int X = Pos.iX() + dx[i];
	int Y = Pos.iY() + dy[i];
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(X, Y, Color8Bit::MagentaA);

	if (Color != _Color)
	{
		// _Color���� ������� �ʴ� ��� Ž��
		int i = CurDir;
		while (true)
		{
			// �� ĭ ���� ��ġ�� ��������
			int nX = X + dx[(i + 2) % 4];
			int nY = Y + dy[(i + 2) % 4];

			// �ݽð�������� ������ Ȯ��
			i = (i + 1) % 4;
			nX += dx[i];
			nY += dy[i];
			Color = UContentsHelper::ColMapImage->GetColor(nX, nY, Color8Bit::MagentaA);
			if (Color == _Color)
			{
				// ������ �ٲ�� ����
				AddActorLocation({ dx[i] * MoveSpeed * _DeltaTime , dy[i] * MoveSpeed * _DeltaTime });
				CurDir = i;
				break;
			}
		}
	}
	else
	{
		// _Color�� ������� ���
		// ���� ������ �̵�
		AddActorLocation({ dx[CurDir] * MoveSpeed * _DeltaTime, dy[CurDir] * MoveSpeed * _DeltaTime });
	}
}

void AFlamer::FallDown(Color8Bit _Color)
{
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color != _Color)
		{
			AddActorLocation(FVector::Down);
		}
		else
		{
			break;
		}
	}
}