// Fill out your copyright notice in the Description page of Project Settings.


#include "RB_DungeonRoom2.h"
#include "Components/ArrowComponent.h" 
#include "Components/StaticMeshComponent.h"

ARB_DungeonRoom2::ARB_DungeonRoom2()
{
	Exit_Arrow_1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Exit_Arrow_1")); 
	Exit_Arrow_2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Exit_Arrow_2")); 
	ClosingWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClosingWall"));

	Exit_Arrow_1->SetupAttachment(ExitPointsFolder); 
	Exit_Arrow_2->SetupAttachment(ExitPointsFolder); 
	ClosingWall->SetupAttachment(GeometryFolder);
}
