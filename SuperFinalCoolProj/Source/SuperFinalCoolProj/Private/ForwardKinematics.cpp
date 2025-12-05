// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardKinematics.h"
#include "ClipController.h"

int ForwardKinematics::UpdateFK(UPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController)
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

	ForwardKinematics::SolvePartialFK(mesh, animationController);

	return 1;
}

int ForwardKinematics::SolvePartialFK(UPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController)
{
	ClipController::AnimationPlayBackData* animation = &animationController->playBackData;

	for (int i = 0; i < animationController->bonesNames->Num(); i++)
	{
		FString* animBonesNames = animationController->bonesNames->GetData();

		int sampleIndex = i + 1;
		if (sampleIndex >= animationController->bonesNames->Num())
		{
			sampleIndex = 0;
		}

		ClipController::AnimationData** pose1 = animationController->data[animBonesNames[i]]->GetData();
		ClipController::AnimationData** pose2 = animationController->data[animBonesNames[sampleIndex]]->GetData();

		int key = animation->currentkeyFrameIndex;
		//lerp position
		FVector deltaLocation = (*pose2[key + 1]->location - *pose1[key]->location)
			* animation->keyFrames[animation->currentkeyFrameIndex]->deltaKeyframe
			+ *pose1[key]->location;

		//lerp scale
		float deltaScale = (pose2[key + 1]->scale - pose1[key]->scale)
			* animation->keyFrames[animation->currentkeyFrameIndex]->deltaKeyframe
			+ pose1[key]->scale;

		//lerp rotation 
		FVector deltaRot =
			(*pose2[key + 1]->rotaion - *pose1[key]->rotaion)
			* animation->keyFrames[animation->currentkeyFrameIndex]->deltaKeyframe
			+ *pose1[key]->rotaion;

		FTransform animPose;
		animPose.SetLocation(deltaLocation);
		animPose.SetRotation(deltaRot.Rotation().Quaternion());
		animPose.SetScale3D(FVector(deltaScale));

		FName parentName = mesh->GetParentBone((FName)animationController->bonesNames->GetData()[i]);
		if (parentName == NAME_None)
		{
			ForwardKinematics::SolveRootFK(mesh, animPose, (FName)animationController->bonesNames->GetData()[i]);
		}
		else
		{
			ForwardKinematics::SolveSingleFK(mesh, animPose, (FName)animationController->bonesNames->GetData()[i], parentName);
		}
	}

	return 1;
}

int ForwardKinematics::SolveRootFK(UPoseableMeshComponent* mesh, FTransform& animPose, FName name)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	mesh->SetBoneTransformByName(name, animPose, EBoneSpaces::ComponentSpace);
	return 1;
}

int ForwardKinematics::SolveSingleFK(UPoseableMeshComponent* mesh, FTransform& animPose, FName name, FName parentName)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	int localBoneIndex = mesh->GetBoneIndex(name);

	//FTransform currentT = mesh->GetBoneSpaceTransforms()[localBoneIndex];

	FTransform parentT = mesh->GetBoneTransformByName(parentName, EBoneSpaces::ComponentSpace);
	FTransform newT((parentT.ToMatrixWithScale() * animPose.ToMatrixWithScale()));

	mesh->SetBoneTransformByName(name, animPose * parentT, EBoneSpaces::ComponentSpace);

	//mesh->SetBoneRotationByName(name, (parentT.GetRotation() * animPoses[boneIndex].GetRotation()).Rotator(), EBoneSpaces::ComponentSpace);
	//mesh->SetBoneLocationByName(name, animPoses[boneIndex].GetLocation(), EBoneSpaces::ComponentSpace);
	return 1;
}