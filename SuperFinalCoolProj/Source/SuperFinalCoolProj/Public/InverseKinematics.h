// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModifiedPoseableMeshComponent.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API InverseKinematics
{
public:
	InverseKinematics() = delete;;
	~InverseKinematics() = delete;;

	static int UpdateEffectors(UModifiedPoseableMeshComponent* mesh, FTransform effectorT);

	static int UpdateFullIK(UModifiedPoseableMeshComponent* mesh);
	static int SolveRootIK(UModifiedPoseableMeshComponent* mesh, FName name);
	static int SolveSingleIK(UModifiedPoseableMeshComponent* mesh, FName name, FName parentName);

private:
	static void ResolvePostEffectorIK(UModifiedPoseableMeshComponent* mesh, FTransform jToObj, FName name);
	static void UpdateLookAt(UModifiedPoseableMeshComponent* mesh, FTransform effectorT);
};
