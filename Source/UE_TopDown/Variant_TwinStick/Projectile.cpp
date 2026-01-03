#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    CollisionSphere->InitSphereRadius(20.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));
    CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(LifeTime);
}

void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectile::FireInDirection(const FVector& Direction)
{
    LaunchDirection = Direction.GetSafeNormal();

    FVector CurrentLocation = GetActorLocation();
    SetActorLocation(CurrentLocation);

    FVector NewVelocity = LaunchDirection * Speed;
    Mesh->AddRelativeRotation(FRotator(0, FMath::RadiansToDegrees(FMath::Atan2(NewVelocity.Y, NewVelocity.X)), 0));
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        // Obsługa zderzenia z wrogiem (będzie uzupełniona później)
        Destroy();
    }
}