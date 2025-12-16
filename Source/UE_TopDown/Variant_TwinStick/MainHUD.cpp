// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_TwinStick/MainHUD.h"
#include "BasePlayerController.h"

AMainHUD::AMainHUD()
{
}

void AMainHUD::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<ABasePlayerController>(GetOwningPlayerController());
}