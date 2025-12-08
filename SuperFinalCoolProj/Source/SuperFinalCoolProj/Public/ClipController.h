// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModifiedPoseableMeshComponent.h"
#include "CoreMinimal.h"
#include "BoneDataNode.h"
#include <map>

/**
 * 
 */
class SUPERFINALCOOLPROJ_API ClipController
{
public:
	ClipController();
	~ClipController();

	void UpdateClipController(float dt, UModifiedPoseableMeshComponent* mesh, USkeletalMeshComponent* skelMeshComp, FTransform lookAtEffector);
	void GetCurvesFromUAsset(USkeletalMeshComponent* skelMeshComp, FString animName);
	BoneDataNode* GetAssetData();

	UAnimSequence* currClip;
	float currClipTime;
	float currClipTimeNormalized;

	struct AnimationData
	{
		FTransform transform;

		FVector* location;
		FVector* rotaion;
		float scale;
	};

	struct AnimationDataController
	{
		//bones used in the animation
		TArray<FString>* bonesNames;

		//animation data realted to the bones
		TMap<FString, TArray<AnimationData*>*> data;
		//TArray<std::pair<FString, AnimationData*>>* data;

		AnimationDataController() : 
			bonesNames(new TArray<FString>), 
			data()
		{
			
		}
	};

private:

	UPROPERTY()
	TMap<FString, AnimationDataController*> animationMap;
	

};
