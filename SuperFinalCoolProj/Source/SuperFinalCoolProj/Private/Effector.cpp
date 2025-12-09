// Fill out your copyright notice in the Description page of Project Settings.


#include "Effector.h"

////Tristan made all this

// Sets default values
AEffector::AEffector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh)"));
	SetRootComponent(mesh);
}

// Called when the game starts or when spawned
void AEffector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

