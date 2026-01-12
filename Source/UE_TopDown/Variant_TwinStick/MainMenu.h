#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameUserSettings.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetComponent.h"
#include "MainMenu.generated.h"

UCLASS()
class UE_TOPDOWN_API AMainMenu : public AActor
{
    GENERATED_BODY()

public:
    AMainMenu();

    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UWidgetComponent* WidgetComponent;

    UFUNCTION()
    void OnNewGameClicked();

    UFUNCTION()
    void OnOptionsClicked();

    UFUNCTION()
    void OnCreditsClicked();

    UFUNCTION()
    void OnBackFromOptionsClicked();

    UFUNCTION()
    void OnBackFromCreditsClicked();

    UFUNCTION()
    void OnQuitClicked();

    // Opcje audio
    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    // Toggle fullscreen
    UFUNCTION()
    void OnFullscreenToggled(bool bIsChecked);
};