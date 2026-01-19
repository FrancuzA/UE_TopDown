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
	
public:
   

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void CreateHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateManaBar(float CurrentMana, float MaxMana);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    class UUserWidget* HUDWidget;


private:
  

    virtual void BeginPlay() override;

};
