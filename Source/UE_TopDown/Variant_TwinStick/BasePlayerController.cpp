// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WBP_PlayerHUD.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"
#include "Components/TextBlock.h" // ✅ DODANE – include dla UTextBlock

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
}

void ABasePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ✅ AKTUALIZUJ TIMER TYLKO GDY JEST HUD I GRACZ ŻYJE:
    if (HUDWidget)
    {
        GameplayTime += DeltaTime;

        // Format: MM:SS
        int32 Minutes = FMath::FloorToInt(GameplayTime / 60.0f);
        int32 Seconds = FMath::FloorToInt(GameplayTime) % 60;

        FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        HUDWidget->SetTimerText(TimerText);
    }
}

void ABasePlayerController::CreateHUD()
{
    if (HUDWidget) return;

    if (HUDWidgetClass && !HUDWidget)
    {
        HUDWidget = CreateWidget<UWBP_PlayerHUD>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport(50);

            if (ABasePlayerCharacter* PC = Cast<ABasePlayerCharacter>(GetPawn()))
            {
                if (PC->Attributes)
                {
                    UpdateHealthBar(PC->Attributes->GetHealth(), PC->Attributes->GetMaxHealth());
                    UpdateManaBar(PC->Attributes->GetMana(), PC->Attributes->GetMaxMana());
                    UpdateScore(PC->Attributes->GetScore());
                }
            }
        }
    }
}

void ABasePlayerController::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
    if (HUDWidget && MaxHealth > 0.f)
    {
        HUDWidget->SetHealthPercent(CurrentHealth / MaxHealth);
    }
}

void ABasePlayerController::UpdateManaBar(float CurrentMana, float MaxMana)
{
    if (HUDWidget && MaxMana > 0.f)
    {
        HUDWidget->SetManaPercent(CurrentMana / MaxMana);
    }
}

void ABasePlayerController::UpdateScore(int32 Score)
{
    if (HUDWidget)
    {
        HUDWidget->SetScoreText(Score);
    }
}

void ABasePlayerController::ShowGameOverScreen(int32 FinalScore)
{
    // Schowaj HUD
    if (HUDWidget)
    {
        HUDWidget->RemoveFromParent();
        HUDWidget = nullptr;
    }

    // Stwórz ekran śmierci
    if (!LooseScreenWidget && LooseScreenWidgetClass)
    {
        LooseScreenWidget = CreateWidget<UUserWidget>(this, LooseScreenWidgetClass);
        if (LooseScreenWidget)
        {
            LooseScreenWidget->AddToViewport(100);

            // ✅ USTAW SCORE W LOOSE SCREEN:
            if (UTextBlock* FinalScoreText = Cast<UTextBlock>(LooseScreenWidget->GetWidgetFromName(TEXT("FinalScoreText"))))
            {
                FinalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), FinalScore)));
            }

            // ✅ USTAW CZAS W LOOSE SCREEN:
            if (UTextBlock* FinalTimeText = Cast<UTextBlock>(LooseScreenWidget->GetWidgetFromName(TEXT("FinalTimeText"))))
            {
                int32 Minutes = FMath::FloorToInt(GameplayTime / 60.0f);
                int32 Seconds = FMath::FloorToInt(GameplayTime) % 60;
                FinalTimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));
            }
        }
    }

    // Włącz UI input
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}