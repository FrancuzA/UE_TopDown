// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE_TOPDOWN_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	/*
public:
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void CreateHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateStaminaBar(float CurrentStamina, float MaxStamina);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    class UUserWidget* HUDWidget;


private:
    void OnInteract();
    void OnAttack();

    virtual void BeginPlay() override;*/

};
