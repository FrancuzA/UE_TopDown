#include "Projectile.h"
#include "GameFramework/DamageType.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->InitSphereRadius(20.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));
    CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

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
        Mesh->SetRelativeRotation(FRotator(0, FMath::RadiansToDegrees(FMath::Atan2(LaunchDirection.Y, LaunchDirection.X)), 0));
    }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AEnemy* _enemy = Cast<AEnemy>(OtherActor);
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        _enemy->TakeDMG( Damage, this);

        Destroy();
    }
}