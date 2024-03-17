#include "KirbyUI.h"
#include "Player.h"

KirbyUI::KirbyUI()
{
}

KirbyUI::~KirbyUI()
{
}

void KirbyUI::BossUIOn()
{
	BossHpCaseUI->ActiveOn();
	BossHpBarUI->ActiveOn();
}

void KirbyUI::MonsterAniOn()
{
	IsMonHpRenderOn = true;
	MonHpRenderer->ActiveOn();
	MonHpRenderer->AnimationReset();	// ChangeAnimation�Լ����� �Ȱ��� �ִϸ��̼� �ٽ� ������ �� �ż� ���� �� �� ����� ��.
	MonHpRenderer->ChangeAnimation("MonHpAni");
}

void KirbyUI::BeginPlay()
{
	SetActorLocation({ 0, 0 });

	TypeNameUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	TypeNameUI->SetImage("nametag_normal.png");
	FVector TypeNameImgScale = TypeNameUI->GetImage()->GetScale();
	TypeNameUI->SetTransform({ TypeNamePos, TypeNameImgScale });
	TypeNameUI->CameraEffectOff();

	KirbyHpCaseUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	KirbyHpCaseUI->SetImage("KirbyHpBarCase.png");
	FVector KHpCaseImgScale = KirbyHpCaseUI->GetImage()->GetScale();
	KirbyHpCaseUI->SetTransform({ HpCasePos + KHpCaseImgScale.Half2D(), KHpCaseImgScale });
	KirbyHpCaseUI->CameraEffectOff();

	KirbyHpUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	KirbyHpUI->SetImage("KirbyHp.png");
	FVector KHpImgScale = KirbyHpUI->GetImage()->GetScale();
	KirbyHpUI->SetTransform({ HpCasePos + KHpCaseImgScale.Half2D(), KHpImgScale });
	KirbyHpUI->CameraEffectOff();

	KirbyLifeImgUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	KirbyLifeImgUI->SetImage("KLifeUI.png");
	FVector KLifeImgScale = KirbyLifeImgUI->GetImage()->GetScale();
	KirbyLifeImgUI->SetTransform({ LifeImgPos + KLifeImgScale.Half2D(), KLifeImgScale });
	KirbyLifeImgUI->CameraEffectOff();

	BossHpCaseUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	BossHpCaseUI->SetImage("MonHpBarCase.png");
	FVector BCaseScale = BossHpCaseUI->GetImage()->GetScale();
	BossHpCaseUI->SetPosition({ WinScale.X - 100.f, KirbyHpCaseUI->GetPosition().Y });	// ���� ü�¹� ���̴� Ŀ�� ü�¹� ���̶� ���ƾ� ��.
	BossHpCaseUI->SetScale(BCaseScale);
	BossHpCaseUI->CameraEffectOff();
	BossHpCaseUI->ActiveOff();

	BossHpBarUI = CreateImageRenderer(EKirbyRenderOrder::UI);
	BossHpBarUI->SetImage("MonHp.png");
	FVector BBarScale = BossHpBarUI->GetImage()->GetScale();
	BossHpBarUI->SetPosition({ WinScale.X - 100.f, KirbyHpCaseUI->GetPosition().Y });
	BossHpBarUI->SetScale(BBarScale);
	BossHpBarUI->CameraEffectOff();
	BossHpBarUI->ActiveOff();

	MonHpRenderer = CreateImageRenderer(EKirbyRenderOrder::UI);
	MonHpRenderer->SetImage("MonHp.png");
	MonHpRenderer->SetPosition({ WinScale.X - 100.f, KirbyHpCaseUI->GetPosition().Y });
	MonHpRenderer->SetScale(BCaseScale);
	MonHpRenderer->CreateAnimation("MonHpAni", "MonHpAnimation", {0,0,1,2,3,3,3,3,3,3}, 0.05f, false);
	MonHpRenderer->ChangeAnimation("MonHpAni");
	MonHpRenderer->CameraEffectOff();
	MonHpRenderer->ActiveOff();

	// Ŀ�� ��� UI
	int Life = Kirby->GetKirbyLife();
	for (int i = 0; i < 2; i++)
	{
		KirbyLifeNumUI[i] = CreateImageRenderer(EKirbyRenderOrder::UI);

		int num = Life / (pow(10, 1-i));
		Life -= num * 10;

		KirbyLifeNumUI[i]->SetImage(std::to_string(num) + ".png");
		FVector KirbyLifeNumScale = KirbyLifeNumUI[i]->GetImage()->GetScale();
		LifeNumUIPos.X += i * 24;
		KirbyLifeNumUI[i]->SetTransform({ LifeNumUIPos + KirbyLifeNumScale.Half2D(), KirbyLifeNumScale });
		KirbyLifeNumUI[i]->CameraEffectOff();
	}
}

void KirbyUI::Tick(float _DeltaTime)
{
	TypeUIUpdate();
	if (true == IsMonHpRenderOn && true == MonHpRenderer->IsCurAnimationEnd())
	{
		IsMonHpRenderOn = false;
		MonHpRenderer->ActiveOff();
	}
}

void KirbyUI::TypeUIUpdate()
{
	// Ŀ�� Ÿ���� �ٲ������ UI ������Ʈ
	if (TypeState != UContentsHelper::KirbyType)
	{
		TypeState = UContentsHelper::KirbyType;
		SetTypeNameUI(TypeState);
	}
}

void KirbyUI::SetKirbyHpUI(int _CurHp)
{
	FVector CurHpScale = KirbyHpUI->GetTransform().GetScale();
	FVector CurHpPos = KirbyHpUI->GetPosition();
	float NewHpScaleX = (183.f / Kirby->GetMaxHp()) * _CurHp;	// 183�� Hp���ҽ� ���� scale
	float NewHpPosX = (CurHpScale.X - NewHpScaleX) / 2.f;

	KirbyHpUI->SetScale({ NewHpScaleX, CurHpScale.Y });
	KirbyHpUI->SetPosition({ CurHpPos.X - NewHpPosX, CurHpPos.Y });
}

void KirbyUI::SetKirbyLifeUI(int _CurLife)
{
	int Life = _CurLife;
	LifeNumUIPos = LifeNumUIDefaultPos;
	for (int i = 0; i < 2; i++)
	{
		int num = Life / (pow(10, 1 - i));
		Life -= num * 10;

		KirbyLifeNumUI[i]->SetImage(std::to_string(num) + ".png");
		FVector KirbyLifeNumScale = KirbyLifeNumUI[i]->GetImage()->GetScale();
		LifeNumUIPos.X += i * 24;
		KirbyLifeNumUI[i]->SetTransform({ LifeNumUIPos + KirbyLifeNumScale.Half2D(), KirbyLifeNumScale });
	}
}

void KirbyUI::SetBossHpUI(int _MaxHp, int _CurHp)
{
	FVector CurHpScale = BossHpBarUI->GetTransform().GetScale();
	FVector CurHpPos = BossHpBarUI->GetPosition();
	float NewHpScaleX = (96.f / _MaxHp) * _CurHp;	// 96�� Hp���ҽ� ���� scale
	float NewHpPosX = (CurHpScale.X - NewHpScaleX) / 2.f;

	BossHpBarUI->SetScale({ NewHpScaleX, CurHpScale.Y });
	BossHpBarUI->SetPosition({ CurHpPos.X - NewHpPosX, CurHpPos.Y });
}

void KirbyUI::SetTypeNameUI(EKirbyType _Type)
{
	std::string resource = "nametag";
	switch (_Type) 
	{
	case EKirbyType::Normal:
		resource += "_normal";
		break;
	case EKirbyType::Fire:
		resource += "_fire";
		break;
	case EKirbyType::Ice:
		resource += "_ice";
		break;
	default:
		break;
	}
	resource += ".png";
	TypeNameUI->SetImage(resource);
}