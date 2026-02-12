// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomBase.h"
#include "RB_DungeonRoom2.generated.h"

/**
 * 
 */
UCLASS()
class TRYFIRSTPERSONROGUE_API ARB_DungeonRoom2 : public ARoomBase
{
	GENERATED_BODY()

public:

	ARB_DungeonRoom2();

	UPROPERTY(Visibleanywhere, Category = "Components")
	UArrowComponent* Exit_Arrow_1;

	UPROPERTY(Visibleanywhere, Category = "Components")
	UArrowComponent* Exit_Arrow_2;

	UPROPERTY(Visibleanywhere, Category = "Components")
	UStaticMeshComponent* ClosingWall;
};
