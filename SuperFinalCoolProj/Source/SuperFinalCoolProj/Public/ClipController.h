// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoneDataNode.h"
#include <map>
#include "Components/PoseableMeshComponent.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API ClipController
{
public:
	ClipController();
	~ClipController();

	void UpdateClipController(float dt);

	void InitAnimationController(FString animationName, float timeBetween);
	BoneDataNode* GetAssetData(UPoseableMeshComponent* mesh, USkeletalMeshComponent* skelMeshComp);


	UAnimSequence* currClip;
	float currClipTime;
	float currClipTimeNormalized;

	struct AnimationData
	{
		FVector* location;
		FVector* rotaion;
		float scale;
	};

	struct AnimationKeyFrame
	{
		float duration;
		float start;
		float end;
		float keyframeTime;
		float deltaKeyframe;

	};

	struct AnimationPlayBackData 
	{
		float clipDuration;
		float clipTime;
		float deltaClipTime;

		TArray<AnimationKeyFrame*> keyFrames;
	};

	struct AnimationDataController
	{
		//bones used in the animation
		TArray<FString>* bonesNames;

		//animation data realted to the bones
		TMap<FString, TArray<AnimationData*>*> data;

		//keyframe and play back data per animation
		AnimationPlayBackData playBackData;

		//TArray<std::pair<FString, AnimationData*>>* data;

		AnimationDataController() : 
			bonesNames(new TArray<FString>), 
			data()
		{
			
		}
	};


	AnimationDataController* GetAnimationControllerByAnimationName(FString name)
	{
		return animationMap[name];
	}

private:

	UPROPERTY()
	TMap<FString, AnimationDataController*> animationMap;
	

};
