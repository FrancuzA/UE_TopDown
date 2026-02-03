// Fill out your copyright notice in the Description page of Project Settings.

#include "Variant_TwinStick/BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"
#include "WBP_PlayerHUD.h" // ← kluczowy include!

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();
    CreateHUD();
}

void ABasePlayerController::CreateHUD()
{
    if (HUDWidgetClass)
    {
        // TWORZYMY WIDGET I RZUTUJEMY NA UWBP_PlayerHUD
        HUDWidget = CreateWidget<UWBP_PlayerHUD>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();

            // Odświeżamy UI z początkowymi wartościami jeśli postać już istnieje
            if (ABasePlayerCharacter* PC = Cast<ABasePlayerCharacter>(GetPawn()))
            {
                if (PC->Attributes)
                {
                    UpdateHealthBar(PC->Attributes->GetHealth(), PC->Attributes->GetMaxHealth());
                    UpdateManaBar(PC->Attributes->GetMana(), PC->Attributes->GetMaxMana());
                    // Jeśli masz UpdateScore – dodaj też:
                    // UpdateScore(PC->Attributes->GetScore());
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