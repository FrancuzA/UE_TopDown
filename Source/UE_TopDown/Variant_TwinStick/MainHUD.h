#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

UCLASS()
class UE_TOPDOWN_API AMainHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMainHUD();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override; // Włącz tick dla aktualizacji czasu

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateHealth(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateMana(float CurrentMana, float MaxMana);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateScore(int32 CurrentScore);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateGameTime(float CurrentTime);

private:
    UPROPERTY()
    class ABasePlayerController* PlayerController;

    UPROPERTY()
    class ABasePlayerCharacter* PlayerCharacter;

    float ElapsedTime = 0.0f;
};