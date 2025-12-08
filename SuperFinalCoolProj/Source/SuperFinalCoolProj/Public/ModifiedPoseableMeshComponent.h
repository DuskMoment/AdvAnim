// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "ModifiedPoseableMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUPERFINALCOOLPROJ_API UModifiedPoseableMeshComponent : public UPoseableMeshComponent
{
	GENERATED_BODY()
	
public:
	void SetBoneSpaceTranformByName(FTransform transform, FName name);
	FTransform* GetBoneSpaceTranformRefByName(FName name);
};
