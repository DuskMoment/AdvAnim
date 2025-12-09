// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *  will made this class bone relations where done based off of names and matches in alist
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

	void SetTransform(FTransform inputTransform);
	void SetLocation(FVector inputLocation);
	void SetRotation(FQuat inputRotation);
	void SetScale(FVector intputScale);

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
