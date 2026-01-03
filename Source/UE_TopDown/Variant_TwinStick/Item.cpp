#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetupAttachment(SceneRoot);
    SetRootComponent(SceneRoot);
}

void AItem::BeginPlay()
{
    Super::BeginPlay();
}

void AItem::Interact_Implementation(AActor* Interactor)
{
    if (ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Interactor))
    {
        PickUp(Player);
    }
}

void AItem::PickUp_Implementation(ABasePlayerCharacter* ByCharacter)
{
    if (!ByCharacter || !ByCharacter->Attributes) return;

    switch (ItemType)
    {
    case EItemType::Health:
        ByCharacter->Attributes->SetHealth(ByCharacter->Attributes->GetHealth() + HealthAmount);
        break;
    case EItemType::Mana:
        ByCharacter->Attributes->SetMana(ByCharacter->Attributes->GetMana() + ManaAmount);
        break;
    case EItemType::Weapon:
        if (WeaponClass)
        {
            ByCharacter->ProjectileClass = WeaponClass;
        }
        break;
    }

    Destroy(); // Natychmiastowe usunięcie przedmiotu
}