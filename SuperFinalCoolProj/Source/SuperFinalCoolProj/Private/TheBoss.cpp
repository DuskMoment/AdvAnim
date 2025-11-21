// Fill out your copyright notice in the Description page of Project Settings.


#include "TheBoss.h"

// Sets default values
ATheBoss::ATheBoss() : AActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("mesh"));
	
	SetRootComponent(mesh);

}

// Called when the game starts or when spawned
void ATheBoss::BeginPlay()
{
	Super::BeginPlay();

	if (mesh == NULL)
	{
		return;
	}

	mesh->GetBoneNames(names);


	
	for (int i = 0; i < names.Num(); i++)
	{
		//get all of the data and place it into a struct
		FTransform trans = mesh->GetBoneTransform(names[i], RTS_Component);
		FVector loc = mesh->GetBoneLocation(names[i]);
		FQuat quat = mesh->GetBoneQuaternion(names[i]);
		FVector scale = trans.GetScale3D();

		BoneDataNode node(trans, loc, quat, scale);
		node.SetBoneName(names[i]);

		node.SetParentName(mesh->GetParentBone(node.GetBoneName()));

		data.Add(node);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone Data loaded %s"), *node.GetBoneName().ToString()));
	}
}

// Called every frame
void ATheBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

