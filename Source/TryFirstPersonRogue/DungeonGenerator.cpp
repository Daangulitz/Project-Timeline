// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "RB_DugneonRoom1.h"
#include "RoomBase.h"
#include "Components/BoxComponent.h"
#include "ClosingWall.h"


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

	FTimerHandle UnusedHandle;

	SpawnStarterRoom();
	
	SpawnNextRoom();
	
	RemoveOverLappingRooms();

	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ADungeonGenerator::CloseUnusedExits, 0.1f, false);
}

void ADungeonGenerator::SpawnStarterRoom()
{
	ARB_DugneonRoom1* SpawnedStarterRoom = this->GetWorld()->SpawnActor<ARB_DugneonRoom1>(StarterRoom);

	SpawnedStarterRoom->SetActorLocation(this->GetActorLocation());

	SpawnedStarterRoom->ExitPointsFolder->GetChildrenComponents(false, Exits);
}

void ADungeonGenerator::SpawnNextRoom()
{
	bCanSpawn = true;
	
	LastestSpawnedRoom = this->GetWorld()->SpawnActor<ARoomBase>(RoomsToBeSpawned[rand() % RoomsToBeSpawned.Num()]);

	USceneComponent* SelectedExit = Exits[rand() % Exits.Num()];

	LastestSpawnedRoom->SetActorLocation(SelectedExit->GetComponentLocation());
	LastestSpawnedRoom->SetActorRotation(SelectedExit->GetComponentRotation());

	RemoveOverLappingRooms();

	if (bCanSpawn) 
	{
		Exits.Remove(SelectedExit);
		TArray<USceneComponent*> LatestRoomExit;
		LastestSpawnedRoom->ExitPointsFolder->GetChildrenComponents(false, LatestRoomExit);
		Exits.Append(LatestRoomExit);
	}

	RoomAmount = RoomAmount - 1;


	if (RoomAmount > 0)
	{
		SpawnNextRoom();
		
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
		bCanSpawn = false;
		RoomAmount = RoomAmount + 1;
		LastestSpawnedRoom->Destroy();
	}
}

void ADungeonGenerator::CloseUnusedExits()
{
	for (USceneComponent* Element : Exits)
	{
		AClosingWall* LatestClosingWallSpawned = GetWorld()->SpawnActor<AClosingWall>(ClosingWall);

		FVector RelativeOffset(-875.0f, 0.0f, 0.0f);
		FVector WorldOffset = Element->GetComponentRotation().RotateVector(RelativeOffset);

		LatestClosingWallSpawned->SetActorLocation(Element->GetComponentLocation() + WorldOffset);
		LatestClosingWallSpawned->SetActorRotation(Element->GetComponentRotation() + FRotator(0.0f,90.0f,0.0f));
	}
}



