// Fill out your copyright notice in the Description page of Project Settings.

#include "WBP_PlayerHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UWBP_PlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();

    if (HealthBar) HealthBar->SetPercent(1.0f);
    if (ManaBar) ManaBar->SetPercent(1.0f);
    if (ScoreText) ScoreText->SetText(FText::FromString(TEXT("Score: 0")));
    if (TimerText) TimerText->SetText(FText::FromString(TEXT("00:00"))); // ✅ STARTOWY CZAS
}

void UWBP_PlayerHUD::SetHealthPercent(float Percent)
{
    if (HealthBar) HealthBar->SetPercent(Percent);
}

void UWBP_PlayerHUD::SetManaPercent(float Percent)
{
    if (ManaBar) ManaBar->SetPercent(Percent);
}

void UWBP_PlayerHUD::SetScoreText(int32 Score)
{
    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
    }
}

void UWBP_PlayerHUD::SetTimerText(const FString& TimeText)
{
    if (TimerText)
    {
        TimerText->SetText(FText::FromString(TimeText));
    }
}