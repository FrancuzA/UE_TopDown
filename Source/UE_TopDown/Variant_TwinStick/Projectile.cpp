// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->InitSphereRadius(20.0f);

    // ✅ KLUCZOWE: kolizja ustawiona programowo
    CollisionSphere->SetCollisionObjectType(ECC_GameTraceChannel1);
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Wrogowie to Pawn!
    CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    CollisionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    CollisionSphere->SetGenerateOverlapEvents(true);

    CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(LifeTime);
}

void AProjectile::FireInDirection(const FVector& Direction)
{
    LaunchDirection = Direction.GetSafeNormal();
    ProjectileMovement->Velocity = LaunchDirection * Speed;

    if (Mesh)
    {
        float Angle = FMath::RadiansToDegrees(FMath::Atan2(LaunchDirection.Y, LaunchDirection.X));
        Mesh->SetRelativeRotation(FRotator(0, Angle, 0));
    }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bHasDealtDamage) return;

    if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy)
    {
        UE_LOG(LogTemp, Log, TEXT("Projectile HIT enemy: %s (Damage: %f)"), *Enemy->GetName(), Damage);
        Enemy->TakeDMG(Damage, GetOwner());
        bHasDealtDamage = true;
    }

    Destroy();
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bHasDealtDamage) return;

    if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy)
    {
        UE_LOG(LogTemp, Log, TEXT("Projectile OVERLAP enemy: %s (Damage: %f)"), *Enemy->GetName(), Damage);
        Enemy->TakeDMG(Damage, GetOwner());
        bHasDealtDamage = true;
        Destroy();
    }
}