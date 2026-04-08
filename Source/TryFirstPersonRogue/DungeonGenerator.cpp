// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "RB_DugneonRoom1.h"
#include "RoomBase.h"
#include "Components/BoxComponent.h"
#include "ClosingWall.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NavMesh/NavMeshBoundsVolume.h"


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

	CloseUnusedExits();

    AActor* FoundVolume = UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass());
    ANavMeshBoundsVolume* NavVolume = Cast<ANavMeshBoundsVolume>(FoundVolume);

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys && NavVolume)
    {
        // Now the types match!
        NavSys->OnNavigationBoundsUpdated(NavVolume);
        NavSys->Build();
    }
}

void ADungeonGenerator::SpawnStarterRoom()
{
	ARB_DugneonRoom1* SpawnedStarterRoom = this->GetWorld()->SpawnActor<ARB_DugneonRoom1>(StarterRoom);

	SpawnedStarterRoom->SetActorLocation(this->GetActorLocation());

	SpawnedStarterRoom->ExitPointsFolder->GetChildrenComponents(false, Exits);
}
void ADungeonGenerator::SpawnNextRoom()
{
    // Use a local variable to track how many we NEED to spawn
    // This prevents the "Ignoring RoomAmount" issue
    int32 TargetCount = RoomAmount;
    int32 SuccessfullySpawned = 0;
    int32 SafetyBreak = 0;
    int32 MaxAttempts = 1000;

    // Remove the recursive call and use this single loop
    while (SuccessfullySpawned < TargetCount && SafetyBreak < MaxAttempts)
    {
        SafetyBreak++;

        if (Exits.Num() == 0) break;

        bCanSpawn = true;

        // Pick random room and exit
        int32 RoomIndex = rand() % RoomsToBeSpawned.Num();
        int32 ExitIndex = rand() % Exits.Num();

        ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(RoomsToBeSpawned[RoomIndex]);
        USceneComponent* SelectedExit = Exits[ExitIndex];

        if (NewRoom && SelectedExit)
        {
            NewRoom->SetActorLocation(SelectedExit->GetComponentLocation());
            NewRoom->SetActorRotation(SelectedExit->GetComponentRotation());

            LastestSpawnedRoom = NewRoom;

            // This destroys the room if it overlaps
            RemoveOverLappingRooms();

            if (bCanSpawn)
            {
                // SUCCESS
                if (Door) {
                    AActor* LatestDoorSpawned = GetWorld()->SpawnActor<AActor>(Door);
                    FVector RelativeOffset(-1000.0f, -120.0f, 0.0f);
                    FVector WorldOffset = SelectedExit->GetComponentRotation().RotateVector(RelativeOffset);
                    LatestDoorSpawned->SetActorLocation(SelectedExit->GetComponentLocation() + WorldOffset);
                    LatestDoorSpawned->SetActorRotation(SelectedExit->GetComponentRotation());
                }

                // Update the Exits list
                Exits.RemoveAt(ExitIndex);
                TArray<USceneComponent*> NewExits;
                NewRoom->ExitPointsFolder->GetChildrenComponents(false, NewExits);
                Exits.Append(NewExits);

                SuccessfullySpawned++;
            }
        }
    }

    // IMPORTANT: Only call this ONCE, after the loop is totally finished.
    // This prevents the "extra walls" issue.
    // CloseUnusedExits(); 
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
    // A TSet automatically handles duplicates. 
    // This ensures we never spawn two walls on the same spot.
    TSet<USceneComponent*> UniqueExits(Exits);

    for (USceneComponent* Element : UniqueExits)
    {
        if (!Element) continue;

        AClosingWall* LatestClosingWallSpawned = GetWorld()->SpawnActor<AClosingWall>(ClosingWall);
        if (LatestClosingWallSpawned)
        {
            FVector RelativeOffset(-1030.0f, -190.0f, 0.0f);
            FVector WorldOffset = Element->GetComponentRotation().RotateVector(RelativeOffset);

            LatestClosingWallSpawned->SetActorLocation(Element->GetComponentLocation() + WorldOffset);

            // Fixed rotation to match the exit
            LatestClosingWallSpawned->SetActorRotation(Element->GetComponentRotation() + FRotator(0.0f, 90.0f, 0.0f));
        }
    }

    // Clear the list so if this is called again, it doesn't double-up
    Exits.Empty();
}



