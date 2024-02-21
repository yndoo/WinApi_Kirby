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

	FlamerRenderer->CreateAnimation("Spin", "Flamer_Spin.png", 0, 3, 0.2f, true);
	FlamerRenderer->CreateAnimation("Hurt", "Flamer_Hurt.png", { 0,0,0,0,0,1 }, 0.3f, false);
	FlamerRenderer->ChangeAnimation("Spin");

	// �ϴ� �ܵ��Ͽ� �������� ��...
	while (true)
	{
		Color8Bit Color = UContentsHelper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color != Color8Bit(255, 255, 0, 0))
		{
			AddActorLocation(FVector::Down);
		}
		else
		{
			break;
		}
	}
	StateChange(EEnemyState::Idle);

	FlamerCollision = CreateCollision(KirbyCollisionOrder::Monster);
	FlamerCollision->SetScale({ 100, 100 });
	FlamerCollision->SetColType(ECollisionType::Rect);
}

void AFlamer::Tick(float _DeltaTime) {
	AActor::Tick(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void AFlamer::StateChange(EEnemyState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EEnemyState::None:
			break;
		case EEnemyState::Idle:
			IdleStart();
			break;
		case EEnemyState::Hurt:
			HurtStart();
			break;
		case EEnemyState::Move:
			MoveStart();
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
	case EEnemyState::None:
		break;
	case EEnemyState::Idle:
		Idle(_DeltaTime);
		break;
	case EEnemyState::Hurt:
		Hurt(_DeltaTime);
		break;
	case EEnemyState::Move:
		Move(_DeltaTime);
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

void AFlamer::Idle(float _DeltaTime)
{
	ColorMove(_DeltaTime, Color8Bit({ 255,255,0,0 }));

	std::vector<UCollision*> Result;
	if (nullptr != FlamerCollision && true == FlamerCollision->CollisionCheck(KirbyCollisionOrder::Player, Result))
	{
		StateChange(EEnemyState::Hurt);
		
		return;
	}
}

void AFlamer::Move(float _DeltaTime)
{
	ColorMove(_DeltaTime, Color8Bit({ 255,0,255,0 }));

	/*std::vector<UCollision*> Result;
	if (nullptr != FlamerCollision && true == FlamerCollision->CollisionCheck(KirbyCollisionOrder::Player, Result))
	{
		StateChange(EEnemyState::Hurt);

		return;
	}*/
}

void AFlamer::Hurt(float _DeltaTime)
{
	// ���߿� ü�� ������ ü���� 0�� �� �׾�� ��(�̿�)
	FVector MyTransform = GetActorLocation();
	Color8Bit Color = UContentsHelper::ColMapImage->GetColor(MyTransform.iX(), MyTransform.iY(), Color8Bit::MagentaA);
	if (Color != Color8Bit(255, 0, 255, 0))
	{
		AddActorLocation(FVector::Down * 100.0f * _DeltaTime);
	}
	else
	{
		StateChange(EEnemyState::Move);
		return;
	}
}

void AFlamer::ColorMove(float _DeltaTime, Color8Bit _Color)
{
	// _Color�� �پ��ִ��� Ȯ���ϱ� ���� �پ��ִ� ���� Ȯ��
	int i = CurDir;
	i = (i + 1) % 4;

	int X = GetActorLocation().iX() + dx[i];
	int Y = GetActorLocation().iY() + dy[i];
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
				AddActorLocation({ dx[i] * 0.17f, dy[i] * 0.17f });
				CurDir = i;
				break;
			}
		}
	}
	else
	{
		// _Color�� ������� ���
		// ���� ������ �̵�
		AddActorLocation({ dx[CurDir] * 0.17f, dy[CurDir] * 0.17f });
	}
}