#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h" // POPRAWNY INCLUDE
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    Grunt,
    Tank,
    Speedster
};

UCLASS()
class UE_TOPDOWN_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemy();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // POPRAWNA DEKLARACJA TakeDamage
    virtual float TakeDamage(
        float DamageAmount,
        const FDamageEvent& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser
    ) override;

    UFUNCTION(BlueprintCallable)
    EEnemyType GetEnemyType() const { return EnemyType; }

    UFUNCTION(BlueprintCallable)
    float GetScoreValue() const { return ScoreValue; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    EEnemyType EnemyType = EEnemyType::Grunt;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    float CurrentHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float DamageOnHit = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float AttackRange = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float AttackCooldown = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float ScoreValue = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop")
    TSubclassOf<class AItem> ItemToDrop;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop")
    float DropChance = 0.3f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* DetectionSphere;

    float LastAttackTime = 0.0f;
    AActor* Target = nullptr;

    virtual void Die(AActor* DamageCauser = nullptr); // DEKLARACJA Z PARAMETREM
    virtual void Attack();
    virtual void DropItem();
};