// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardKinematics.h"

int ForwardKinematics::UpdateFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPose, ClipController::AnimationDataController* controller)
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

	ForwardKinematics::SolvePartialFK(mesh, animPose, *controller);

	return 1;
}

int ForwardKinematics::SolvePartialFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPoses, ClipController::AnimationDataController& data)
{
	TArray<FName> names;
	mesh->GetBoneNames(names);

	for (int i = 0; i < names.Num(); i++)
	{
		FName parentName = mesh->GetParentBone(names[i]);
		if (parentName == NAME_None)
		{
			ForwardKinematics::SolveRootFK(mesh, animPoses, i, names[i]);
		}
		else
		{
			ForwardKinematics::SolveSingleFK(mesh, animPoses, i, names[i], parentName);
		}
	}

	return 1;
}

int ForwardKinematics::SolveRootFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPose, int index, FName name)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	mesh->SetBoneTransformByName(name, animPose[index], EBoneSpaces::ComponentSpace);
	return 1;
}

int ForwardKinematics::SolveSingleFK(UPoseableMeshComponent* mesh, TArray<FTransform>& animPose, int index, FName childName, FName parentName)
{
	//FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	//int localBoneIndex = mesh->GetBoneIndex(name);

	//FTransform currentT = mesh->GetBoneSpaceTransforms()[localBoneIndex];

	FTransform parentT = mesh->GetBoneTransformByName(parentName, EBoneSpaces::ComponentSpace);
	FTransform newT((parentT.ToMatrixWithScale() * animPose[index].ToMatrixWithScale()));

	mesh->SetBoneTransformByName(childName, animPose[index] * parentT, EBoneSpaces::ComponentSpace);

	//mesh->SetBoneRotationByName(name, (parentT.GetRotation() * animPoses[boneIndex].GetRotation()).Rotator(), EBoneSpaces::ComponentSpace);
	//mesh->SetBoneLocationByName(name, animPoses[boneIndex].GetLocation(), EBoneSpaces::ComponentSpace);
	return 1;
}

int ForwardKinematics::BlendPoses(ClipController::ClipKeyframes frames, int keyFrame, ClipController::AnimationDataController* input, TArray<FTransform>& outPose)
{

	TArray<FString> boneNames = *input->bonesNames;
	
	//get the current frame
	ClipController::KeyFrames key = frames.frames[keyFrame];
	float interpolation = key.keyFrameParam;

	outPose.Empty();
	//somthing w

	for (int i = 0; i < input->bonesNames->Num(); i++)
	{
		FTransform lerpedPos;
		FString name = boneNames[i];
		ClipController::AnimationData* pose1 = input->data[name]->GetData()[keyFrame];
		ClipController::AnimationData* pose2 = input->data[name]->GetData()[keyFrame + 1];

		lerpedPos.SetLocation(FMath::Lerp(pose2->transform.GetLocation(), pose1->transform.GetLocation(), interpolation));

		FVector rotated = FMath::Lerp(pose2->transform.GetRotation().Euler(), pose1->transform.GetRotation().Euler(), interpolation);
		FRotator blendRotator(rotated.X, rotated.Y, rotated.Z);
		lerpedPos.SetRotation(blendRotator.Quaternion());

		lerpedPos.SetScale3D(FMath::Lerp(pose2->transform.GetScale3D(), pose1->transform.GetScale3D(), interpolation));

		outPose.Add(lerpedPos);
	}
	return 0;
}
