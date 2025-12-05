// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoneDataNode.h"
#include <map>
#include "ModifiedPoseableMeshComponent.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API ClipController
{
public:
	ClipController();
	~ClipController();

	

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
		float speed = 0.5f;
		float clipDuration;
		float clipTime;
		float deltaClipTime;

		int currentkeyFrameIndex = 0;

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

		//maybe add a lerped pose
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

	void UpdateClipController(float dt, AnimationPlayBackData* const contrl, UModifiedPoseableMeshComponent* mesh, USkeletalMeshComponent* skelMeshComp);
private:

	UPROPERTY()
	TMap<FString, AnimationDataController*> animationMap;
	

};
