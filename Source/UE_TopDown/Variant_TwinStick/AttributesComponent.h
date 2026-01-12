// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedDelegate, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedDelegate, float, CurrentScore);

USTRUCT(BlueprintType)
struct FManaCost
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    float ManaRegenRate = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    float ManaCost_Attack = 20.0f;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

    // Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    float Health;

    // Mana
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    float MaxMana = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
    float Mana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    FManaCost ManaCost;

    bool bIsRegeneratingMana = true;

public:	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    float Score = 0.0f;

    
    UPROPERTY(BlueprintAssignable, Category = "Score")
    FOnScoreChangedDelegate OnScoreChanged;

    // Health Functions
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

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(float Points);

    // Stamina Functions
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

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FOnDeathDelegate OnDeath;

    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FOnAttributeChangedDelegate OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Mana")
    FOnAttributeChangedDelegate OnManaChanged;
		
};
