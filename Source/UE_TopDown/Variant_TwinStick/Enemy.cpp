// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerCharacter.h"
#include "Components/CapsuleComponent.h"
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

    if (bIsDead) return; // ✅ NIE TICKUJ MARTWYCH
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

// ✅ GŁÓWNA FUNKCJA – ZABIJ OD RAZU
void AEnemy::TakeDMG(float DamageAmount, AActor* DamageCauser)
{
    if (bIsDead) return;

    UE_LOG(LogTemp, Warning, TEXT("Enemy %s taking %f damage. Health: %f -> %f"),
        *GetName(), DamageAmount, CurrentHealth, CurrentHealth - DamageAmount);

    CurrentHealth -= DamageAmount;

    // ✅ ZABIJ OD RAZU gdy HP <= 0 (z marginesem na float)
    if (CurrentHealth <= 0.1f && !bIsDead)
    {
        bIsDead = true;

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

        // Drop itemu
        if (FMath::FRand() < DropChance && ItemToDrop)
        {
            DropItem();
            UE_LOG(LogTemp, Warning, TEXT("✅ Dropped item: %s"), *ItemToDrop->GetName());
        }

        // ✅ ZNIKNIJ OD RAZU – BEZ SetLifeSpan(), BEZ Die()
        Destroy();
    }
}

void AEnemy::Attack()
{
    if (bIsDead || !Target) return; // ✅ NIE ATAKUJ MARTWYCH

    ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Target);
    if (Player)
    {
        Player->GetHit_Implementation(DamageOnHit);
    }
}

void AEnemy::DropItem()
{
    if (bIsDead) return;

    FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AItem>(ItemToDrop, SpawnLocation, SpawnRotation, SpawnParams);
}