// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasePlayerCharacter.h"
#include "InteractionInterface.h"
#include "InteractionComponent.generated.h"

class UKismetSystemLibrary;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_TOPDOWN_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> IgnoreActors;

	UPROPERTY()
	float TraceSphereRadius = 10.f;

	void TryInteract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void SphereTrace(FHitResult& SphereHit);	
};
