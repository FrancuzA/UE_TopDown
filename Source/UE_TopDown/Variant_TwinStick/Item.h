// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Item.generated.h"

UCLASS()
class UE_TOPDOWN_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	// Implementacja interfejsu
	virtual void Interact_Implementation(AActor* Interactor);

	// Funkcja do podnoszenia
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Item")
	void PickUp(ABasePlayerCharacter* ByCharacter);
	virtual void PickUp_Implementation(ABasePlayerCharacter* ByCharacter);
};
