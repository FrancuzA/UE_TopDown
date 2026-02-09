// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_PlayerHUD.generated.h"

UCLASS()
class UE_TOPDOWN_API UWBP_PlayerHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetHealthPercent(float Percent);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetManaPercent(float Percent);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetScoreText(int32 Score);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetTimerText(const FString& TimeText);

protected:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ManaBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimerText; // ✅ DODANE – MUSI SIĘ NAZYWAĆ "TimerText" W BLUEPRINT!
};