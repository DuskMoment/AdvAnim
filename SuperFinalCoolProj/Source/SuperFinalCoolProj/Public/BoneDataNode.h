// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API BoneDataNode
{
public:
	BoneDataNode();
	BoneDataNode(FTransform trans, FVector location, FQuat rotation, FVector scale);
	FTransform GetTransform();
	FVector GetLocation();
	FQuat GetRotation();
	FVector GetScale();
	~BoneDataNode();

	void SetBoneName(FName name);
	void SetParentName(FName name);

	FName GetBoneName();
	FName GetParentName();

private:
	FTransform trans;
	FVector location;
	FQuat rotation;
	FVector scale;
	FName boneName;
	FName parentBoneName;
	FTransform parentTransform;
};
