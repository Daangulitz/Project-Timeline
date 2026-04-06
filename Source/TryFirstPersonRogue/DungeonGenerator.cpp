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
	RoomsSpawnedCount = 0;

	SpawnStarterRoom();
	
	SpawnNextRoom();
	
	RemoveOverLappingRooms();

	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ADungeonGenerator::CloseUnusedExits, 0.1f, true);
}

void ADungeonGenerator::SpawnStarterRoom()
{
	ARB_DugneonRoom1* SpawnedStarterRoom = this->GetWorld()->SpawnActor<ARB_DugneonRoom1>(StarterRoom);

	SpawnedStarterRoom->SetActorLocation(this->GetActorLocation());

	SpawnedStarterRoom->ExitPointsFolder->GetChildrenComponents(false, Exits);
}
void ADungeonGenerator::SpawnNextRoom()
{
    // 1. Setup a Safety Check to prevent infinite loops
    int32 MaxAttempts = 1000;
    int32 CurrentAttempt = 0;

    // 2. Loop until we have spawned the desired amount of rooms
    // We check RoomAmount > 0 because your original code decrements it
    while (RoomAmount > 0 && CurrentAttempt < MaxAttempts)
    {
        CurrentAttempt++;

        // Safety: If we run out of exits, we can't spawn anymore
        if (Exits.Num() == 0) break;

        bCanSpawn = true;

        // Pick random room and exit
        int32 RoomIndex = rand() % RoomsToBeSpawned.Num();
        int32 ExitIndex = rand() % Exits.Num();

        ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(RoomsToBeSpawned[RoomIndex]);
        USceneComponent* SelectedExit = Exits[ExitIndex];

        if (!NewRoom) continue;

        NewRoom->SetActorLocation(SelectedExit->GetComponentLocation());
        NewRoom->SetActorRotation(SelectedExit->GetComponentRotation());

        // Update your global pointer for the overlap check
        LastestSpawnedRoom = NewRoom;

        // 3. Check for overlaps
        // IMPORTANT: Ensure RemoveOverLappingRooms() DOES NOT call SpawnNextRoom() 
        // and DOES NOT increment RoomAmount anymore.
        RemoveOverLappingRooms();

        if (bCanSpawn)
        {
            // Spawn Door logic
            if (Door) {
                AActor* LatestDoorSpawned = GetWorld()->SpawnActor<AActor>(Door);
                FVector RelativeOffset(-1000.0f, -120.0f, 0.0f);
                FVector WorldOffset = SelectedExit->GetComponentRotation().RotateVector(RelativeOffset);
                LatestDoorSpawned->SetActorLocation(SelectedExit->GetComponentLocation() + WorldOffset);
                LatestDoorSpawned->SetActorRotation(SelectedExit->GetComponentRotation());
            }

            // Successfully spawned: Update the list of available exits
            Exits.RemoveAt(ExitIndex);
            TArray<USceneComponent*> NewExits;
            NewRoom->ExitPointsFolder->GetChildrenComponents(false, NewExits);
            Exits.Append(NewExits);

            // ONLY decrement when a room is actually placed successfully
            RoomAmount--;
        }
        // If bCanSpawn is false, the loop just repeats and tries again 
        // with a different random room/exit combo.
    }

    // 4. Once the loop finishes, clean up
    CloseUnusedExits();
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

		FVector RelativeOffset(-1030.0f, -190.0f, 0.0f);
		FVector WorldOffset = Element->GetComponentRotation().RotateVector(RelativeOffset);

		LatestClosingWallSpawned->SetActorLocation(Element->GetComponentLocation() + WorldOffset);
		LatestClosingWallSpawned->SetActorRotation(Element->GetComponentRotation() + FRotator(0.0f,90.0f,0.0f));
	}
}



