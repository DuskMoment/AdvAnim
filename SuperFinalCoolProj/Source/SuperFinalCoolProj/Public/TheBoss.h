// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "BoneDataNode.h"
#include "Heirarchy.h"
#include "GameFramework/Actor.h"
#include "TheBoss.generated.h"

UCLASS()
class SUPERFINALCOOLPROJ_API ATheBoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATheBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisableAnywhere);
	

	TArray<FName> names;
	TArray<BoneDataNode> data;

	Heirarchy* pH;

	UPROPERTY(EditAnywhere)
	UPoseableMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* skelMesh;

	UPROPERTY(EditAnywhere)
	UAnimSequence* animClip;

private:
	void ApplyChangeToMesh(FName boneName);
};
