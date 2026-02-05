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
    GetCharacterMovement()->GravityScale = 1.0f; // Normalna grawitacja dla 3D

    // Komponenty:
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

    if (!Target) return;

    // Pobierz AIController dynamicznie (bez cache):
    AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController) return;

    float DistanceXY = FVector::DistXY(GetActorLocation(), Target->GetActorLocation());

    if (DistanceXY > AttackRange)
    {
        isAttacking = false;
        AIController->MoveToActor(Target, 50.0f); // 50.0f = promień akceptacji
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

    // Teraz mamy AIController!
    AAIController* AIController = Cast<AAIController>(NewController);
    if (AIController)
    {
        UE_LOG(LogTemp, Log, TEXT("Enemy %s POSSESSED by AIController"), *GetName());

        // Opcjonalnie: rozpocznij śledzenie gracza
        TArray<AActor*> Players;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerCharacter::StaticClass(), Players);
        if (Players.Num() > 0)
        {
            Target = Players[0];
            UE_LOG(LogTemp, Log, TEXT("Enemy %s targeting player: %s"), *GetName(), *Target->GetName());
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
        Die(DamageCauser);
    }

    return DamageAmount;
}

void AEnemy::Die(AActor* DamageCauser) 
{
    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetActorTickEnabled(false);

    if (DamageCauser && DamageCauser->GetInstigatorController())
    {
        ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(DamageCauser->GetInstigator());
        if (Player && Player->Attributes)
        {
            Player->Attributes->AddScore(ScoreValue);
        }
    }

    if (FMath::FRand() < DropChance && ItemToDrop)
    {
        DropItem();
    }

    SetLifeSpan(1.0f); 
}

void AEnemy::Attack()
{
    if (Target)
    {
        UE_LOG(LogTemp, Display, TEXT("Attack function called"));

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