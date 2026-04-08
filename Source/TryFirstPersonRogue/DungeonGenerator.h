// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

class ARB_DugneonRoom1;
class ARoomBase;
class AClosingWall;

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

	UPROPERTY(EditAnywhere, Category = "Unused Exits Closing Wall")
	TSubclassOf<AClosingWall> ClosingWall;

	UPROPERTY(EditAnywhere, Category = "Used Exits Doors")
	TSubclassOf<AActor> Door;
	
	UPROPERTY(EditAnywhere, Category = "Dungeon Info")
	int32 RoomAmount;

	UFUNCTION()
	void OnNavMeshGenerationFinished(ANavigationData* NavData);

	ARoomBase* LastestSpawnedRoom;

	bool bCanSpawn;
	int32 RoomsSpawnedCount = 0;
	FTimerHandle GenerationTimerHandle;
	 
	TArray<USceneComponent*> Exits;

	void SpawnStarterRoom();

	void SpawnNextRoom();

	void RemoveOverLappingRooms();

	void CloseUnusedExits();
};
