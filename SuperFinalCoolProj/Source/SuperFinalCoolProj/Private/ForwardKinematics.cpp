// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardKinematics.h"
#include "ClipController.h"

int ForwardKinematics::UpdateFK(UModifiedPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController, Heirarchy& h)
{


	ClipController::AnimationPlayBackData* animation = &animationController->playBackData;

	//these meshes are not the same?
	TArray<FName> names;
	mesh->GetBoneNames(names);

	FString* animBonesNames = animationController->bonesNames->GetData();
	int numberOfNames = animationController->bonesNames->Num();

	UE_LOG(LogTemp, Warning, TEXT("numberOfNames %i"), numberOfNames);

	for (int i = 0; i < numberOfNames; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("numberOfNames %s"), *animBonesNames[i]);
	}

	UE_LOG(LogTemp, Warning, TEXT("numberOfBaseBoneNames %i"), h.basePose.Num());

	for (int i = 0; i < names.Num(); i++)
	{
		if (mesh->GetBoneIndex((FName)names[i]) != -1)
		{
			//get the data at that bone
			ClipController::AnimationData** pose1 = animationController->data[names[i].ToString()]->GetData();

			//just use the first key frame for now
			int key = 0;

			//get pose location
			FVector deltaLocation = *pose1[key]->location;

			//deltaLocation += h.basePose[i].GetLocation();

			//get pose scale
			float deltaScale = pose1[key]->scale;

			//get pose rotation in degrees
			FVector deltaRot = *pose1[key]->rotaion;

			FRotator DeltaRotator(deltaRot.X, deltaRot.Y, deltaRot.Z);

			FQuat DeltaRotationQuat = DeltaRotator.Quaternion();

			FQuat testquat = DeltaRotationQuat * h.basePose[i].GetRotation();

			deltaRot.X = h.basePose[i].GetRotation().X;
			deltaRot.Y = h.basePose[i].GetRotation().Y;
			deltaRot.Z = h.basePose[i].GetRotation().Z;

			FTransform animPose;

			animPose.SetLocation(deltaLocation);
			//animPose.SetRotation(h.testBase.GetRotation());
			//animPose.SetRotation(deltaRot.Rotation().Quaternion());
			animPose.SetRotation(testquat);
			animPose.SetScale3D(mesh->GetBoneSpaceTransforms()[i].GetScale3D());

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
				FString::Printf(TEXT("animPose x:%s %f y:%f z:%f"), *animBonesNames[i], animPose.GetLocation().X, animPose.GetLocation().Y, animPose.GetLocation().Z));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
				FString::Printf(TEXT("basePose x:%s %f y:%f z:%f"), *animBonesNames[i], h.basePose[i].GetLocation().X, h.basePose[i].GetLocation().Y, h.basePose[i].GetLocation().Z));



			TArray<FName> unrealBoneNames;
			mesh->GetBoneNames(unrealBoneNames);

			//update the mesh
			mesh->SetBoneSpaceTranformByName(animPose, (FName)names[i]); //the HTR has a lleft and right eye which is causing the it to crash
			//mesh->SetBoneSpaceTranformByName(animOis)

			//else do nothing with it its not real

		}
	}

	mesh->bDisplayBones = true;
	mesh->RefreshBoneTransforms();
	//ForwardKinematics::SolvePartialFK(mesh, animationController, h);


	return 1;
}

int ForwardKinematics::SolvePartialFK(UPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController, Heirarchy& h)
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
		ClipController::AnimationData** pose2 = animationController->data[animBonesNames[i]]->GetData();

		int key = animation->currentkeyFrameIndex;
		//lerp position
		FVector deltaLocation = (*pose2[key + 1]->location - *pose1[key]->location)
			* animation->keyFrames[key]->deltaKeyframe
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

		/*int baseBoneIndex = mesh->SkeletalMesh->RefSkeleton.FindBoneIndex((FName)animBonesNames[i]);
		if (baseBoneIndex != -1)
		{
			FTransform refBoneBaseTransform = mesh->SkeletalMesh->RefSkeleton.GetRefBonePose()[baseBoneIndex];
			animPose.SetLocation(animPose.GetLocation() - refBoneBaseTransform.GetLocation());
			animPose.SetRotation(animPose.GetRotation() * refBoneBaseTransform.GetRotation());
			animPose.SetScale3D(FVector(deltaScale));
		}*/

		FName parentName = h.FindBoneByName((FName)animBonesNames[i])->GetParentName(); //gets the parent bone from the heriarchy
		//FName parentName = mesh->GetParentBone((FName)animationController->bonesNames->GetData()[i]);
		if (parentName == NAME_None)
		{
			ForwardKinematics::SolveRootFK(mesh, animPose, (FName)animationController->bonesNames->GetData()[i]);
		}
		else
		{
			ForwardKinematics::SolveSingleFK(mesh, animPose, (FName)animBonesNames[i], parentName);
		}
	}

	return 1;
}

int ForwardKinematics::SolveRootFK(UPoseableMeshComponent* mesh, FTransform& animPose, FName name)
{
	FCompactPoseBoneIndex boneIndex = FCompactPoseBoneIndex(mesh->GetBoneIndex(name));
	mesh->SetBoneTransformByName(name, animPose, EBoneSpaces::ComponentSpace);


	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, 
	//		FString::Printf(TEXT("animPose x:%f y:%f z:%f"), animPose.GetLocation().X, animPose.GetLocation().Y, animPose.GetLocation().Z));
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