// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardKinematics.h"

//ForwardKinematics::ForwardKinematics()
//{
//}
//
//ForwardKinematics::~ForwardKinematics()
//{
//}

int ForwardKinematics::SolvePartialFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses)
{
	TArray<FName> names;
	mesh->GetBoneNames(names);

	for (int i = 0; i < names.Num(); i++)
	{
		FName parentName = mesh->GetParentBone(names[i]);
		if (parentName == NAME_None)
		{
			ForwardKinematics::SolveRootFK(mesh, animPoses, names[i]);
		}
		else
		{
			ForwardKinematics::SolveSingleFK(mesh, animPoses, names[i], parentName);
		}
	}

	return 1;
}

int ForwardKinematics::SolveRootFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	mesh->SetBoneTransformByName(name, animPoses[boneIndex], EBoneSpaces::ComponentSpace);
	return 1;
}

int ForwardKinematics::SolveSingleFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name, FName parentName)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	FTransform currentT = mesh->GetBoneTransformByName(name, EBoneSpaces::ComponentSpace);
	FTransform parentT = mesh->GetBoneTransformByName(parentName, EBoneSpaces::ComponentSpace);
	//mesh->SetBoneTransformByName(name, parentT * animPoses[boneIndex], EBoneSpaces::ComponentSpace);

	mesh->GetBoneSpaceTransforms();

	mesh->SetBoneRotationByName(name, (parentT.GetRotation() * animPoses[boneIndex].GetRotation()).Rotator(), EBoneSpaces::ComponentSpace);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone: %s, Transform: %s"), *name.ToString(), *animPoses[boneIndex].GetLocation().ToString()));
	mesh->SetBoneLocationByName(name, animPoses[boneIndex].GetLocation(), EBoneSpaces::ComponentSpace);
	return 1;
}
