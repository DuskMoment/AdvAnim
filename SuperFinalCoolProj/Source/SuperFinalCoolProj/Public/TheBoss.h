// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModifiedPoseableMeshComponent.h"
#include "ClipController.h"
#include "CoreMinimal.h"
#include "BoneDataNode.h"
#include "Heirarchy.h"
#include "BlendTree.h"
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
	UModifiedPoseableMeshComponent* mesh;

	USkeletalMeshComponent* skelMeshComp;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* skelMesh;

	UPROPERTY(EditAnywhere)
	UAnimSequence* animClip;
	//UAnimationCont* animation;

	UPROPERTY(EditAnywhere)
	AActor* lookAtEffector;

	ClipController* controller;

	BlendTree* blendTree;

	UPROPERTY(EditAnywhere)
	UClass* animInst;

	UPROPERTY(EditAnywhere)
	ATheBoss* blendSkel1;

	UPROPERTY(EditAnywhere)
	ATheBoss* blendSkel2;

	UPROPERTY(EditAnywhere)
	float lerpU;

private:
	void ApplyChangeToMesh(FName boneName);
};
