// Fill out your copyright notice in the Description page of Project Settings.


#include "TheBoss.h"

// Sets default values
ATheBoss::ATheBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATheBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATheBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

