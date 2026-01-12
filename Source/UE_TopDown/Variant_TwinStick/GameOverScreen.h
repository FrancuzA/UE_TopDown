#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameOverScreen.generated.h"

UCLASS()
class UE_TOPDOWN_API AGameOverScreen : public AActor
{
    GENERATED_BODY()

public:
    AGameOverScreen();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void SetFinalScore(int32 Score);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UWidgetComponent* WidgetComponent;

    UFUNCTION()
    void OnRestartClicked();

    UFUNCTION()
    void OnMainMenuClicked();
};