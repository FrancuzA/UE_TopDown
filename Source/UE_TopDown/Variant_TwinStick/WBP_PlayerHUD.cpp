#include "WBP_PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWBP_PlayerHUD::SetHealthPercent(float Percent)
{
    if (HealthBar) HealthBar->SetPercent(Percent);
}

void UWBP_PlayerHUD::SetManaPercent(float Percent)
{
    if (ManaBar) ManaBar->SetPercent(Percent);
}

void UWBP_PlayerHUD::SetScore(int32 Score)
{
    if (ScoreText) ScoreText->SetText(FText::AsNumber(Score));
}

void UWBP_PlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();
}