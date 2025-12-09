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

	//used to update the clip controller
	void UpdateClipController(float dt, UModifiedPoseableMeshComponent* mesh, USkeletalMeshComponent* skelMeshComp, FTransform lookAtEffector);

	BoneDataNode* GetAssetData();

	UAnimSequence* currClip;
	float currClipTime;
	float currClipTimeNormalized;

	//stores local animation data
	struct AnimationData
	{
		FVector* location;
		FVector* rotaion;
		float scale;
	};

	//strose a map of joints to animation poese
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
	//contains the map of animationnames to dataControllers
	TMap<FString, AnimationDataController*> animationMap;
	

};
