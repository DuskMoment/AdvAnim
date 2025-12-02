// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoneDataNode.h"
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

	BoneDataNode* GetAssetData(UPoseableMeshComponent& mesh);

	UAnimSequence* currClip;
	float currClipTime;
	float currClipTimeNormalized;
};
