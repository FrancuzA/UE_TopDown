// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_TwinStick/BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();
    CreateHUD();
}


void ABasePlayerController::CreateHUD()
{
    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }
}

void ABasePlayerController::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
    // This will be implemented in Blueprint
}

void ABasePlayerController::UpdateStaminaBar(float CurrentStamina, float MaxStamina)
{
    // This will be implemented in Blueprint
}


void ABasePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("IA_Interact", IE_Pressed, this, &ABasePlayerController::OnInteract);
    InputComponent->BindAction("IA_Attack", IE_Pressed, this, &ABasePlayerController::OnAttack);
}

void ABasePlayerController::OnInteract()
{

    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
    if (PlayerCharacter)
    {
        PlayerCharacter->Interact();
    }
}

void ABasePlayerController::OnAttack()
{
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
    if (PlayerCharacter)
    {

        PlayerCharacter->Attack();
    }
}