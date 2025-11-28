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
	size_t boneCount;
public:
	Heirarchy();
	Heirarchy(TArray<BoneDataNode>* arrayToCopy);
	Heirarchy(size_t boneCount);


	size_t GetBoneCount();
	~Heirarchy();

	BoneDataNode* FindBoneByName(FName name);
};
