// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API ForwardKinematics
{
public:
	ForwardKinematics() = delete;
	~ForwardKinematics() = delete;

	static int SolvePartialFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses);
	static int SolveRootFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name);
	static int SolveSingleFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name, FName parentName);
};
