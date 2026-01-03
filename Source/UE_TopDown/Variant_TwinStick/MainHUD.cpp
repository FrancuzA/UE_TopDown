#include "MainHUD.h"
#include "BasePlayerController.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"

AMainHUD::AMainHUD()
{
    PrimaryActorTick.bCanEverTick = true; // Aktywuj tick
}

void AMainHUD::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = Cast<ABasePlayerController>(GetOwningPlayerController());
    if (PlayerController && PlayerController->GetPawn())
    {
        PlayerCharacter = Cast<ABasePlayerCharacter>(PlayerController->GetPawn());
    }
}

void AMainHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerCharacter || !PlayerCharacter->Attributes) return;

    // Aktualizacja UI
    UpdateHealth(PlayerCharacter->Attributes->GetHealth(), PlayerCharacter->Attributes->GetMaxHealth());
    UpdateMana(PlayerCharacter->Attributes->GetMana(), PlayerCharacter->Attributes->GetMaxMana());
    UpdateScore(FMath::RoundToInt(PlayerCharacter->Attributes->Score));

    // Aktualizacja czasu
    ElapsedTime += DeltaTime;
    UpdateGameTime(ElapsedTime);
}