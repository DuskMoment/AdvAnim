// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardKinematics.h"

//Tristan made this and Will editted for other potential implimentation
int ForwardKinematics::UpdateFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses)
{
	////GetAnimationPose() does concat already
	// 
	//TArray<FName> names;
	//mesh->GetBoneNames(names);

	//for (int i = 0; i < names.Num(); i++)
	//{
	//	int baseBoneIndex = mesh->SkeletalMesh->RefSkeleton.FindBoneIndex(names[i]);

	//	FTransform boneBaseLocalTransform = mesh->SkeletalMesh->RefSkeleton.GetRefBonePose()[baseBoneIndex];
	//	
	//	FCompactPoseBoneIndex animBoneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(names[i]));
	//	
	//	FVector t(0.0f, 0.0f, 0.0f);

	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone: %s, Transform: %s"), *names[i].ToString(), *boneBaseLocalTransform.GetTranslation().ToString()));
	//	////Concat
	//	animPoses[animBoneIndex].SetTranslation(animPoses[animBoneIndex].GetTranslation() + boneBaseLocalTransform.GetTranslation());
	//	animPoses[animBoneIndex].SetRotation((animPoses[animBoneIndex].GetRotation() * boneBaseLocalTransform.GetRotation()));
	//	animPoses[animBoneIndex].SetScale3D(animPoses[animBoneIndex].GetScale3D() * boneBaseLocalTransform.GetScale3D());
	//	
	//	//mesh->GetBoneSpaceTransforms();
	//}

	ForwardKinematics::SolvePartialFK(mesh, animPoses);

	return 1;
}

//Tristan made this
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

//Tristan made this
int ForwardKinematics::SolveRootFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	mesh->SetBoneTransformByName(name, animPoses[boneIndex], EBoneSpaces::ComponentSpace);
	return 1;
}

//Tristan made this
int ForwardKinematics::SolveSingleFK(UPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name, FName parentName)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	int localBoneIndex = mesh->GetBoneIndex(name);

	//FTransform currentT = mesh->GetBoneSpaceTransforms()[localBoneIndex];

	FTransform parentT = mesh->GetBoneTransformByName(parentName, EBoneSpaces::ComponentSpace);

	mesh->SetBoneTransformByName(name, animPoses[boneIndex] * parentT, EBoneSpaces::ComponentSpace);

	//mesh->SetBoneRotationByName(name, (parentT.GetRotation() * animPoses[boneIndex].GetRotation()).Rotator(), EBoneSpaces::ComponentSpace);
	//mesh->SetBoneLocationByName(name, animPoses[boneIndex].GetLocation(), EBoneSpaces::ComponentSpace);
	return 1;
}