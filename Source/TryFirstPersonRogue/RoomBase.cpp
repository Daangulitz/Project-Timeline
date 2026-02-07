


#include "RoomBase.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ARoomBase::ARoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	GeometryFolder = CreateDefaultSubobject<USceneComponent>(TEXT("GeometryFolder"));
	OverlapFolder = CreateDefaultSubobject<USceneComponent>(TEXT("OverlapFolder"));

	Cube_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_1"));
	Cube_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_2"));
	Cube_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_3"));
	Cube_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_4"));
	Cube_5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_5"));
	Cube_6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_6"));
	Cube_7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_7"));
	Cube_8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube_8"));

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Component"));


	DefaultSceneRoot->SetupAttachment(RootComponent);
	GeometryFolder->SetupAttachment(DefaultSceneRoot);
	OverlapFolder->SetupAttachment(DefaultSceneRoot);

	Arrow->SetupAttachment(DefaultSceneRoot);

	Cube_1->SetupAttachment(GeometryFolder);
	Cube_2->SetupAttachment(GeometryFolder);
	Cube_3->SetupAttachment(GeometryFolder);
	Cube_4->SetupAttachment(GeometryFolder);
	Cube_5->SetupAttachment(GeometryFolder);
	Cube_6->SetupAttachment(GeometryFolder);
	Cube_7->SetupAttachment(GeometryFolder);
	Cube_8->SetupAttachment(GeometryFolder);
	Floor->SetupAttachment(GeometryFolder);

	BoxCollision->SetupAttachment(OverlapFolder);

	Arrow->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

