// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

// ====== DEKLARACJE DELEGATÓW — MUSZĄ BYĆ PRZED KLASĄ! ======
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, Mana, float, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, Score);

USTRUCT(BlueprintType)
struct FManaCost
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    float ManaRegenRate = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    float ManaCost_Attack = 20.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_TOPDOWN_API UAttributesComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UAttributesComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ===== HEALTH =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    float Health;

    // ===== MANA =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    float MaxMana = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
    float Mana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    FManaCost ManaCost;

    bool bIsRegeneratingMana = true;

    // ===== SCORE =====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    int32 Score = 0;

public:
    // ===== DELEGATY (EVENTY) =====
    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FOnDeathDelegate OnDeath;

    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Mana")
    FOnManaChanged OnManaChanged;

    UPROPERTY(BlueprintAssignable, Category = "Score")
    FOnScoreChanged OnScoreChanged;

    // ===== HEALTH FUNCTIONS =====
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void SetHealth(float NewHealth);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void ApplyDamage(float Damage);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    bool IsAlive() const { return Health > 0.0f; }

    // ===== SCORE FUNCTIONS =====
    UFUNCTION(BlueprintCallable, Category = "Score")
    int32 GetScore() const { return Score; }

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Points);

    // ===== MANA FUNCTIONS =====
    UFUNCTION(BlueprintCallable, Category = "Mana")
    float GetMana() const { return Mana; }

    UFUNCTION(BlueprintCallable, Category = "Mana")
    float GetMaxMana() const { return MaxMana; }

    UFUNCTION(BlueprintCallable, Category = "Mana")
    void SetMana(float NewMana);

    UFUNCTION(BlueprintCallable, Category = "Mana")
    bool CanPayManaCost(float Cost) const { return Mana >= Cost; }

    UFUNCTION(BlueprintCallable, Category = "Mana")
    void PayMana(float Cost);

    UFUNCTION(BlueprintCallable, Category = "Mana")
    void StartManaRegeneration() { bIsRegeneratingMana = true; }

    UFUNCTION(BlueprintCallable, Category = "Mana")
    void StopManaRegeneration() { bIsRegeneratingMana = false; }
};