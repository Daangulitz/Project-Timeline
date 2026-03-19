// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

class ARB_DugneonRoom1;
class ARoomBase;

UCLASS()
class TRYFIRSTPERSONROGUE_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, Category = "Rooms")
	TSubclassOf<ARB_DugneonRoom1> StarterRoom;

	UPROPERTY(EditAnywhere, Category = "Rooms")
	TArray<TSubclassOf<ARoomBase>> RoomsToBeSpawned;

	TArray<USceneComponent*> Exits;

	void SpawnStarterRoom();

	void SpawnNextRoom();
};
