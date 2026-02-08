// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WBP_PlayerHUD.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"
#include "Kismet/GameplayStatics.h"

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // ✅ RESET INPUTU NA START (kluczowa linijka!)
    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;

    CreateHUD();
}
void ABasePlayerController::CreateHUD()
{
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

void ABasePlayerController::ShowGameOverScreen(int32 FinalScore)
{
    // 1. Schowaj HUD gry
    if (HUDWidget)
    {
        HUDWidget->RemoveFromParent();
        HUDWidget = nullptr;
    }

    // 2. Stwórz ekran przegranej JEDNORAZOWO
    if (!LooseScreenWidget && LooseScreenWidgetClass)
    {
        LooseScreenWidget = CreateWidget<UUserWidget>(this, LooseScreenWidgetClass);
        if (LooseScreenWidget)
        {
            LooseScreenWidget->AddToViewport(100);
        }
    }

    // 3. Zablokuj input gry, odblokuj UI
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}