// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "BasePlayerCharacter.h"
#include "AttributesComponent.h"
#include "Projectile.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = false;

    // ✅ ROOT:
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    // ✅ MESH (wizualny, BEZ kolizji):
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetupAttachment(SceneRoot);
    MeshComp->SetCollisionProfileName(TEXT("NoCollision")); // ✅ KLUCZOWE: mesh NIE blokuje gracza!
    MeshComp->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f)); // Mniejszy item

    // ✅ SPHERE KOLIZJI (tylko do wykrywania overlap):
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(SceneRoot);
    InteractionSphere->InitSphereRadius(50.0f); // Promień wykrywania
    InteractionSphere->SetCollisionProfileName(TEXT("OverlapAll")); // ✅ KLUCZOWE: overlap z wszystkim
    InteractionSphere->SetGenerateOverlapEvents(true); // ✅ Włącz eventy overlap
    InteractionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AItem::OnOverlapBegin);
}

void AItem::BeginPlay()
{
    Super::BeginPlay();

    // ✅ OBRACAJ ITEM dla efektu:
    if (MeshComp)
    {
        MeshComp->SetSimulatePhysics(false);
        MeshComp->SetEnableGravity(false);
    }
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
    if (!ByCharacter || !ByCharacter->Attributes)
    {
        UE_LOG(LogTemp, Warning, TEXT("PickUp failed: no player or attributes"));
        return;
    }

    switch (ItemType)
    {
    case EItemType::Health:
    {
        float NewHealth = ByCharacter->Attributes->GetHealth() + HealthAmount;
        ByCharacter->Attributes->SetHealth(NewHealth);
        UE_LOG(LogTemp, Log, TEXT("✅ Picked up HEALTH +%f (New HP: %f)"), HealthAmount, NewHealth);
        break;
    }
    case EItemType::Mana:
    {
        float NewMana = ByCharacter->Attributes->GetMana() + ManaAmount;
        ByCharacter->Attributes->SetMana(NewMana);
        UE_LOG(LogTemp, Log, TEXT("✅ Picked up MANA +%f (New Mana: %f)"), ManaAmount, NewMana);
        break;
    }
    case EItemType::Weapon:
    {
        if (WeaponClass)
        {
            ByCharacter->CurrentProjectileClass = WeaponClass;
            UE_LOG(LogTemp, Log, TEXT("✅ Picked up WEAPON UPGRADE!"));
        }
        break;
    }
    }

    Destroy();
}

// ✅ AUTOMATYCZNE PODNOSZENIE:
void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // ✅ DEBUG: sprawdź czy event się wywołuje
    UE_LOG(LogTemp, Log, TEXT("Item overlap detected with: %s"), *OtherActor->GetName());

    if (ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(OtherActor))
    {
        UE_LOG(LogTemp, Log, TEXT("✅ Player touched item - picking up!"));
        PickUp(Player);
    }
}