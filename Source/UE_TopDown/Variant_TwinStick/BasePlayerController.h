// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

// Forward declaration � nie potrzebujemy pe�nego include'u w .h
class UWBP_PlayerHUD;

/**
 *
 */
UCLASS()
class UE_TOPDOWN_API ABasePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void CreateHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateManaBar(float CurrentMana, float MaxMana);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateUIOnDeath();

    // Zachowujemy Twoj� oryginaln� w�a�ciwo�� z Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    // Silnie typowany wska�nik do widgetu (wa�ne!)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UWBP_PlayerHUD* HUDWidget;
};