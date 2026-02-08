// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WBP_PlayerHUD.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
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
    if (HUDWidget)
    {
        HUDWidget->RemoveFromParent();
        HUDWidget = nullptr;
    }

    if (!LooseScreenWidget && LooseScreenWidgetClass)
    {
        LooseScreenWidget = CreateWidget<UUserWidget>(this, LooseScreenWidgetClass);
        if (LooseScreenWidget)
        {
            LooseScreenWidget->AddToViewport(100);
        }
    }

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}