// Fill out your copyright notice in the Description page of Project Settings.


#include "Heirarchy.h"

Heirarchy::Heirarchy()
{
}

//this might want to be a const pointer 
Heirarchy::Heirarchy(TArray<BoneDataNode>*  arrayToCopy)
{
	pBones = arrayToCopy->GetData();
	boneCount = arrayToCopy->Num();
}

Heirarchy::Heirarchy(size_t boneCount)
{
	this->boneCount = boneCount;
}

Heirarchy::~Heirarchy()
{
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
