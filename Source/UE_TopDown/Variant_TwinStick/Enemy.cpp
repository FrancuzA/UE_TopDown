#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerCharacter.h"
#include "Item.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->MaxWalkSpeed = 250.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;

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
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Target)
    {
        FVector Direction = Target->GetActorLocation() - GetActorLocation();
        Direction.Z = 0;

        if (Direction.Size() > AttackRange)
        {
            FVector MovementDirection = Direction.GetSafeNormal();
            AddMovementInput(MovementDirection);
        }
        else
        {
            if (GetWorld()->GetTimeSeconds() - LastAttackTime > AttackCooldown)
            {
                Attack();
                LastAttackTime = GetWorld()->GetTimeSeconds();
            }
        }
    }
}

float AEnemy::TakeDamage(
    float DamageAmount,
    const FDamageEvent& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser
)
{
    CurrentHealth -= DamageAmount;

    if (CurrentHealth <= 0.0f)
    {
        Die(DamageCauser); // PRZEKAZUJEMY DAMAGECAUSER
    }

    return DamageAmount;
}

void AEnemy::Die(AActor* DamageCauser) // IMPLEMENTACJA Z PARAMETREM
{
    // Natychmiastowe dezaktywowanie
    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetActorTickEnabled(false);

    // Przyznaj punkty
    if (DamageCauser && DamageCauser->GetInstigatorController())
    {
        ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(DamageCauser->GetInstigator());
        if (Player && Player->Attributes)
        {
            Player->Attributes->AddScore(ScoreValue);
        }
    }

    // Drop itemu
    if (FMath::FRand() < DropChance && ItemToDrop)
    {
        DropItem();
    }

    // Proste zniszczenie po 1s (BEZ TIMERÓW)
    SetLifeSpan(1.0f); 
}

void AEnemy::Attack()
{
    if (Target)
    {
        // ZADAJ OBRAŻENIA GRACZOWI
        UGameplayStatics::ApplyDamage(Target, DamageOnHit, nullptr, this, nullptr);
    }
}

void AEnemy::DropItem()
{
    FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AItem>(ItemToDrop, SpawnLocation, SpawnRotation, SpawnParams);
}