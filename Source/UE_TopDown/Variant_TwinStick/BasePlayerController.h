// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UWBP_PlayerHUD;
class UUserWidget;
class UTextBlock; // ✅ DODANE – forward declaration

UCLASS()
class UE_TOPDOWN_API ABasePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override; // ✅ DODANE – deklaracja Tick

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void CreateHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateManaBar(float CurrentMana, float MaxMana);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateScore(int32 Score);

    UFUNCTION(BlueprintCallable, Category = "Game Over")
    void ShowGameOverScreen(int32 FinalScore);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UWBP_PlayerHUD* HUDWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Over")
    TSubclassOf<UUserWidget> LooseScreenWidgetClass;

private:
    UPROPERTY()
    UUserWidget* LooseScreenWidget = nullptr;

    float GameplayTime = 0.0f; // ✅ CZAS ROZGRYWKI
};