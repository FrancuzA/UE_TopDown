// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "Item.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = 250.0f;
    GetCharacterMovement()->GravityScale = 1.0f;

    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(GetRootComponent());
    DetectionSphere->InitSphereRadius(500.0f);
    DetectionSphere->SetCollisionProfileName(TEXT("NoCollision"));

    CurrentHealth = MaxHealth;
    ScoreValue = 20.0f; // ✅ 20 PUNKTÓW ZA KAŻDEGO WROGA
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;

    UE_LOG(LogTemp, Log, TEXT("Enemy %s spawned"), *GetName());
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsDead) return;
    if (!Target) return;

    AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController) return;

    float DistanceXY = FVector::DistXY(GetActorLocation(), Target->GetActorLocation());

    if (DistanceXY > AttackRange)
    {
        isAttacking = false;
        AIController->MoveToActor(Target, 50.0f);
    }
    else
    {
        AIController->StopMovement();

        if (GetWorld()->GetTimeSeconds() - LastAttackTime > AttackCooldown)
        {
            isAttacking = true;
            Attack();
            LastAttackTime = GetWorld()->GetTimeSeconds();
        }
    }
}

void AEnemy::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    AAIController* AIController = Cast<AAIController>(NewController);
    if (AIController)
    {
        UE_LOG(LogTemp, Log, TEXT("Enemy %s POSSESSED by AIController"), *GetName());

        TArray<AActor*> Players;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerCharacter::StaticClass(), Players);
        if (Players.Num() > 0)
        {
            Target = Players[0];
            UE_LOG(LogTemp, Log, TEXT("Enemy %s targeting player: %s"), *GetName(), *Target->GetName());
        }
    }
}
void AEnemy::TakeDMG(float DamageAmount, AActor* DamageCauser)
{
    if (bIsDead) return;

    UE_LOG(LogTemp, Warning, TEXT("Enemy %s taking %f damage. Health: %f -> %f"),
        *GetName(), DamageAmount, CurrentHealth, CurrentHealth - DamageAmount);

    CurrentHealth -= DamageAmount;

    if (CurrentHealth <= 0.1f && !bIsDead)
    {
        // Nalicz punkty
        if (DamageCauser && DamageCauser->GetInstigatorController())
        {
            ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(DamageCauser->GetInstigator());
            if (Player && Player->Attributes)
            {
                Player->Attributes->AddScore(ScoreValue);
                UE_LOG(LogTemp, Warning, TEXT("✅ Awarded %f score for killing %s"), ScoreValue, *GetName());
            }
        }

        // ✅ DROP ITEMU ZANIM USTAWIMY bIsDead:
        if (FMath::FRand() < DropChance)
        {
            DropItem();
        }

        // ✅ TERAZ USTAW bIsDead:
        bIsDead = true;
        Destroy();
    }
}


void AEnemy::Attack()
{
    if (bIsDead || !Target) return;

    ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Target);
    if (Player)
    {
        Player->GetHit_Implementation(DamageOnHit);
    }
}

void AEnemy::DropItem()
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("DropItem: No world"));
        return;
    }

    float Rand = FMath::FRand();
    EItemType DropType;
    TSubclassOf<AItem> ItemToSpawn = nullptr;
    float HealthAmount = 0.0f;
    float ManaAmount = 0.0f;
    TSubclassOf<AProjectile> WeaponClass = nullptr;

    if (Rand < 0.6f)
    {
        DropType = EItemType::Health;
        ItemToSpawn = HealthItemClass;
        HealthAmount = 25.0f;
    }
    else if (Rand < 0.9f)
    {
        DropType = EItemType::Mana;
        ItemToSpawn = ManaItemClass;
        ManaAmount = 30.0f;
    }
    else
    {
        DropType = EItemType::Weapon;
        ItemToSpawn = WeaponUpgradeItemClass;
        WeaponClass = UpgradeProjectileClass;
    }

    if (!ItemToSpawn)
    {
        UE_LOG(LogTemp, Error, TEXT("DropItem: ItemToSpawn is NULL! Check blueprint settings."));
        return;
    }

    // ✅ POPRAWIONY SPAWN (50 zamiast 100):
    FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 30);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AItem* DroppedItem = GetWorld()->SpawnActor<AItem>(ItemToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

    if (DroppedItem)
    {
        DroppedItem->ItemType = DropType;
        DroppedItem->HealthAmount = HealthAmount;
        DroppedItem->ManaAmount = ManaAmount;
        DroppedItem->WeaponClass = WeaponClass;

        UE_LOG(LogTemp, Log, TEXT("✅ SUCCESS: Dropped item %s at %s"),
            *DroppedItem->GetName(),
            *SpawnLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ FAILED: SpawnActor returned NULL! Location: %s"),
            *SpawnLocation.ToString());
    }
}