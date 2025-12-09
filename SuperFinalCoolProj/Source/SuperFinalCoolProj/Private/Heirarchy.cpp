// Fill out your copyright notice in the Description page of Project Settings.

//will made all of these functions
#include "Heirarchy.h"

Heirarchy::Heirarchy()
{
}

//this might want to be a const pointer 
Heirarchy::Heirarchy(TArray<BoneDataNode>*  arrayToCopy)
{
	pBones = arrayToCopy->GetData();
	boneCount = arrayToCopy->Num();

	root = pBones;

	CaptureBasePose();
}

Heirarchy::Heirarchy(size_t boneCount)
{
	this->boneCount = boneCount;
}

//cause mem leak PLEASE FIX
void Heirarchy::CaptureBasePose()
{
	//capture the current position and make a copy then call it the base pose
	TArray<BoneDataNode> copy;

	for (int i = 0; i < boneCount; i++)
	{
		//make a copy
		copy.Add(*(pBones + i));
	}

	pBasePose = copy.GetData();
}

Heirarchy::~Heirarchy()
{
	delete pBones;
	delete pBasePose;
}

const BoneDataNode* Heirarchy::GetBoneBasePose()
{
	return pBasePose;
}

BoneDataNode* Heirarchy::FindBoneByName(FName name)
{

	if(pBones == NULL)
	{
		return NULL;
	}

	//look for the bone in the bones array
	for (int i = 0; i < boneCount; i++)
	{
		if (pBones[i].GetBoneName() == name)
		{
			return pBones + i;
		}
	}

	//not found
	return new BoneDataNode();
}

size_t Heirarchy::GetBoneCount()
{
	return boneCount;
}
