// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class UE_TOPDOWN_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    AProjectile();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void FireInDirection(const FVector& Direction);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float Speed = 1000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float Damage = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float LifeTime = 5.0f;

    FVector LaunchDirection;

private:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    bool bHasDealtDamage = false;
};