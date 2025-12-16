// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedDelegate, float, CurrentValue, float, MaxValue);

USTRUCT(BlueprintType)
struct FStaminaCost
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaRegenRate = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaCost_Attack = 20.0f;
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

    // Stamina
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float MaxStamina = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
    float Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    FStaminaCost StaminaCost;

    bool bIsRegeneratingStamina = true;

public:	
	

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

    // Stamina Functions
    UFUNCTION(BlueprintCallable, Category = "Stamina")
    float GetStamina() const { return Stamina; }

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    float GetMaxStamina() const { return MaxStamina; }

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void SetStamina(float NewStamina);

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    bool CanPayStaminaCost(float Cost) const { return Stamina >= Cost; }

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void PayStamina(float Cost);

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void StartStaminaRegeneration() { bIsRegeneratingStamina = true; }

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void StopStaminaRegeneration() { bIsRegeneratingStamina = false; }

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FOnDeathDelegate OnDeath;

    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FOnAttributeChangedDelegate OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Stamina")
    FOnAttributeChangedDelegate OnStaminaChanged;
		
};
