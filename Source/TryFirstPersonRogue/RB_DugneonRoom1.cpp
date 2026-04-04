// Fill out your copyright notice in the Description page of Project Settings.


#include "RB_DugneonRoom1.h"
#include "Components/ArrowComponent.h"

ARB_DugneonRoom1::ARB_DugneonRoom1()
{
	Exit_Arrow_1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Exit_Arrow_1"));

	Exit_Arrow_1->SetupAttachment(ExitPointsFolder);
}
