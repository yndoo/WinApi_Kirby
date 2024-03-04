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

	MonsterRenderer = CreateImageRenderer(EKirbyRenderOrder::Monster);
	MonsterRenderer->SetImage("Flamer_Spin.png");
	MonsterRenderer->SetTransform({ {0,0}, {220, 220} });
	MonsterRenderer->SetTransColor(Color8Bit::Magenta);

	MonsterRenderer->CreateAnimation("Spin", "Flamer_Spin.png", 0, 3, 0.2f, true);
	MonsterRenderer->CreateAnimation("Damaged", "Flamer_Damaged.png", { 0,0,0,0,1 }, 0.2f, false);
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
	MonsterHelper::Tick(_DeltaTime);
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
		// ���߿� Start�Լ��� ��������ϴ� ��ü�� ��� FallDown�� �ٽ� ����.
		FallDown(MoveColor);
	}
	ColorLineMove(_DeltaTime, MoveColor);

	std::vector<UCollision*> Result;
	if (nullptr != MonsterCollision && true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
	{
		StateChange(EEnemyState::Damaged);
		return;
	}
}

void AFlamer::DamagedStart()
{
	MonsterRenderer->ChangeAnimation("Damaged");
}
void AFlamer::Damaged(float _DeltaTime)
{
	// ���߿� ü�� ������ ü���� 0�� �� �׾�� ��(�̿�)

	// �ٴڿ� ������ �� Move
	if (MonsterRenderer->IsCurAnimationEnd() == true)
	{
		FallDown(Color8Bit::MagentaA);
		StateChange(EEnemyState::Move);
		return;
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

	// �� �Ѿ�� ���ֱ�
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
	// ���ԵǴٰ� Ŀ�� ���� �浹���� ���
	std::vector<UCollision*> Result;
	if (true == MonsterCollision->CollisionCheck(EKirbyCollisionOrder::Player, Result))
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

