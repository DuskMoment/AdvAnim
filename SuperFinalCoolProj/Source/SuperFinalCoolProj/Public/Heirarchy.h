// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BoneDataNode.h"
#include "CoreMinimal.h"


/**
 * 
 */
class SUPERFINALCOOLPROJ_API Heirarchy
{
private:
	BoneDataNode* pBones;
	BoneDataNode* pBasePose;
	BoneDataNode* root;

	size_t boneCount;

public:
	Heirarchy();
	Heirarchy(TArray<BoneDataNode>* arrayToCopy);
	Heirarchy(size_t boneCount);

	void CaptureBasePose();

	size_t GetBoneCount();
	~Heirarchy();

	const BoneDataNode* GetBoneBasePose();
	BoneDataNode* FindBoneByName(FName name);

	TArray<FTransform> basePose;
};
