// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h"
#include "AttributesComponent.h"
#include "BasePlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInteractionComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class UE_TOPDOWN_API ABasePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FVector GetCameraLocation();

	UFUNCTION(BlueprintCallable)
	FVector GetCameraForwardVector();

	UFUNCTION(BlueprintCallable)
	void Interact();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributesComponent* Attributes;

	virtual void GetHit_Implementation(FVector HitLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaCost_Attack = 20.0f;

	UFUNCTION(BlueprintCallable)
	void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* MappingContext;

	/** Input actions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* ViewCamera;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	UFUNCTION()
	void OnHealthChanged(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void OnStaminaChanged(float CurrentStamina, float MaxStamina);

	UFUNCTION()
	void OnDeath();

};
