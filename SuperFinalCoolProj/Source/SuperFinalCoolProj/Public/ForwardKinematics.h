// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModifiedPoseableMeshComponent.h"
#include "ClipController.h"
#include "Heirarchy.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API ForwardKinematics
{
public:
	ForwardKinematics() = delete;
	~ForwardKinematics() = delete;

	static int UpdateFK(UModifiedPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController, ClipController::AnimationDataController* baseAnimationController, Heirarchy& h);
	static int SolvePartialFK(UPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController, Heirarchy& h);
	static int SolveRootFK(UPoseableMeshComponent* mesh, FTransform& animPose, FName name);
	static int SolveSingleFK(UPoseableMeshComponent* mesh, FTransform& animPose, FName name, FName parentName);
};
