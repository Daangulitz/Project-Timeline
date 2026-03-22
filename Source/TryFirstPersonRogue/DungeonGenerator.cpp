// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "RB_DugneonRoom1.h"
#include "RoomBase.h"
#include "Components/BoxComponent.h"


// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	SpawnStarterRoom();
	
	SpawnNextRoom();
}

void ADungeonGenerator::SpawnStarterRoom()
{
	ARB_DugneonRoom1* SpawnedStarterRoom = this->GetWorld()->SpawnActor<ARB_DugneonRoom1>(StarterRoom);

	SpawnedStarterRoom->SetActorLocation(this->GetActorLocation());

	SpawnedStarterRoom->ExitPointsFolder->GetChildrenComponents(false, Exits);
}

void ADungeonGenerator::SpawnNextRoom()
{
	LastestSpawnedRoom = this->GetWorld()->SpawnActor<ARoomBase>(RoomsToBeSpawned[rand() % RoomsToBeSpawned.Num()]);

	USceneComponent* SelectedExit = Exits[rand() % Exits.Num()];

	LastestSpawnedRoom->SetActorLocation(SelectedExit->GetComponentLocation());
	LastestSpawnedRoom->SetActorRotation(SelectedExit->GetComponentRotation());

	Exits.Remove(SelectedExit);
	TArray<USceneComponent*> LatestRoomExit;
	LastestSpawnedRoom->ExitPointsFolder->GetChildrenComponents(false, LatestRoomExit);
	Exits.Append(LatestRoomExit);

	RoomAmount = RoomAmount - 1;

	if (RoomAmount > 0)
	{
		SpawnNextRoom();
		RemoveOverLappingRooms
	}
}

void ADungeonGenerator::RemoveOverLappingRooms()
{
	TArray<USceneComponent*> OverlappedRooms;
	LastestSpawnedRoom->OverlapFolder->GetChildrenComponents(false, OverlappedRooms);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	for (USceneComponent* Element : OverlappedRooms) {
		Cast<UBoxComponent>(Element)->GetOverlappingComponents(OverlappingComponents);
	}

	for (UPrimitiveComponent* Element : OverlappingComponents) {
		LastestSpawnedRoom->Destroy();
	}
}



