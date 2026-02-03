#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_PlayerHUD.generated.h"

UCLASS()
class UE_TOPDOWN_API UWBP_PlayerHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetHealthPercent(float Percent);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetManaPercent(float Percent);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetScore(int32 Score);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ManaBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;
};