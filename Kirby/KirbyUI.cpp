#include "KirbyUI.h"
#include "Player.h"

KirbyUI::KirbyUI()
{
}

KirbyUI::~KirbyUI()
{
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
}

void KirbyUI::TypeUIUpdate()
{
	// 커비 타입이 바뀌었으면 UI 업데이트
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
	float NewHpScaleX = (183.f / Kirby->GetMaxHp()) * _CurHp;	// 183은 Hp리소스 가로 scale
	float NewHpPosX = (CurHpScale.X - NewHpScaleX) / 2.f;

	KirbyHpUI->SetScale({ NewHpScaleX, CurHpScale.Y });
	KirbyHpUI->SetPosition({ CurHpPos.X - NewHpPosX, CurHpPos.Y });
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
	default:
		break;
	}
	resource += ".png";
	TypeNameUI->SetImage(resource);
}