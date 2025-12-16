// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE_TOPDOWN_API AMainHUD : public AHUD
{
	GENERATED_BODY()
public:
    AMainHUD();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateHealth(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateStamina(float CurrentStamina, float MaxStamina);

private:
    UPROPERTY()
    class ABasePlayerController* PlayerController;

};
