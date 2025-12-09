// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClipController.h"
#include "Components/PoseableMeshComponent.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API ForwardKinematics
{
public:
	ForwardKinematics() = delete;
	~ForwardKinematics() = delete;

	static int UpdateFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPose, ClipController::AnimationDataController* controller);
	static int SolvePartialFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPose,ClipController::AnimationDataController& controller);
	static int SolveRootFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPose, int index, FName name);
	static int SolveSingleFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPose, int index, FName childName, FName parentName);
	static int BlendPoses(ClipController::ClipKeyframes frames, int keyFrame, ClipController::AnimationDataController* input, TArray<FTransform>& outPose);
};
