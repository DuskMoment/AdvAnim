// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifiedPoseableMeshComponent.h"

void UModifiedPoseableMeshComponent::SetBoneSpaceTranformByName(FTransform transform, FName name)
{
	int index = GetBoneIndex(name);
	BoneSpaceTransforms[index] = transform;
}

FTransform* UModifiedPoseableMeshComponent::GetBoneSpaceTranformRefByName(FName name)
{
	int index = GetBoneIndex(name);
	return &BoneSpaceTransforms[index];
}
