// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardKinematics.h"
#include "ClipController.h"

int ForwardKinematics::UpdateFK(UModifiedPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController, ClipController::AnimationDataController* baseAnimationController, Heirarchy& h)
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
		if (!animationController->data.Contains(names[i].ToString()))
		{
			UE_LOG(LogTemp, Warning, TEXT("this bones does not exist in the map %s"), *names[i].ToString());
		}
		else if (mesh->GetBoneIndex((FName)names[i]) != -1)
		{
			//get the data at that bone
			ClipController::AnimationData** pose1 = animationController->data[names[i].ToString()]->GetData();
			ClipController::AnimationData** poseBase = baseAnimationController->data[names[i].ToString()]->GetData();

			//just use the first key frame for now
			int key = 0;

			//get pose location
			FVector deltaLocation = *pose1[key]->location;
			FVector baseAnimLocation = *poseBase[0]->location;

			deltaLocation = h.basePose[i].GetTranslation() + (deltaLocation - baseAnimLocation);

			//get pose scale
			float deltaScale = pose1[key]->scale;

			//get pose rotation in degrees
			FVector deltaRot = *pose1[key]->rotaion;
			FVector baseAnimRot = *poseBase[0]->rotaion;

			//FRotator DeltaRotator(h.basePose[i].GetRotation().Euler().X + deltaRot.X - baseAnimRot.X, h.basePose[i].GetRotation().Euler().Y + deltaRot.Y - baseAnimRot.Y, h.basePose[i].GetRotation().Euler().Z + deltaRot.Z - baseAnimRot.Z);

			FRotator DeltaRotator(deltaRot.X - baseAnimRot.X, deltaRot.Y - baseAnimRot.Y, deltaRot.Z - baseAnimRot.Z);

			FQuat DeltaRotationQuat = DeltaRotator.Quaternion();

			FQuat testquat = DeltaRotationQuat * h.basePose[i].GetRotation();

			//FName parentName = h.FindBoneByName(names[i])->GetParentName(); //gets the parent bone from the heriarchy

			////VQTranslation = (  ( T(A).X - T(B).X ),  ( T(A).Y - T(B).Y ), ( T(A).Z - T(B).Z), 0.f );
			//deltaLocation = deltaLocation - ParentTransform.Translation;

			//// Inverse RotatedTranslation
			//TransformVectorRegister VInverseParentRot = VectorQuaternionInverse(ParentTransform.Rotation);
			//TransformVectorRegister VR = VectorQuaternionRotateVector(VInverseParentRot, VQTranslation);

			//// Translation = 1/S(B)
			//Translation = VectorMultiply(VR, VSafeScale3D);

			//// Rotation = Q(B)(-1) * Q(A)	
			//Rotation = VectorQuaternionMultiply2(VInverseParentRot, Rotation);

			FTransform animPose;

			animPose.SetLocation(deltaLocation);
			animPose.SetRotation(testquat);
			animPose.SetScale3D(h.basePose[i].GetScale3D());

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
				FString::Printf(TEXT("animPose x:%s %f y:%f z:%f"), *names[i].ToString(), deltaLocation.X, deltaLocation.Y, deltaLocation.Z));
			/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
				FString::Printf(TEXT("basePose x:%s %f y:%f z:%f"), *names[i].ToString(), baseAnimLocation.X, baseAnimLocation.Y, baseAnimLocation.Z));*/

			//update the mesh
			mesh->SetBoneSpaceTranformByName(animPose, (FName)names[i]); //the HTR has a lleft and right eye which is causing the it to crash
			//mesh->SetBoneTransformByName(names[i], animPose, EBoneSpaces::ComponentSpace);

			//else do nothing with it its not real

			FName parentName = h.FindBoneByName(names[i])->GetParentName(); //gets the parent bone from the heriarchy
			//FName parentName = mesh->GetParentBone((FName)animationController->bonesNames->GetData()[i]);
			/*if (parentName == NAME_None)
			{
				ForwardKinematics::SolveRootFK(mesh, animPose, names[i]);
			}
			else
			{
				ForwardKinematics::SolveSingleFK(mesh, animPose, names[i], parentName);
			}*/
		}
	}

	//mesh->bDisplayBones = true;
	//mesh->RefreshBoneTransforms();
	//ForwardKinematics::SolvePartialFK(mesh, animationController, h);


	return 1;
}

int ForwardKinematics::SolvePartialFK(UPoseableMeshComponent* mesh, ClipController::AnimationDataController* animationController, Heirarchy& h)
{
	ClipController::AnimationPlayBackData* animation = &animationController->playBackData;

	TArray<FName> names;
	mesh->GetBoneNames(names);
	for (int i = 0; i < names.Num(); i++)
	{

		if (animationController->data.Contains(names[i].ToString()))//skip the bone not in the map
		{
			FString* animBonesNames = animationController->bonesNames->GetData();

			int sampleIndex = i + 1;
			if (sampleIndex >= animationController->bonesNames->Num())
			{
				sampleIndex = 0;
			}

			//issue with the top of head bone not being in the animation
			ClipController::AnimationData** pose1 = animationController->data[names[i].ToString()]->GetData();
			ClipController::AnimationData** pose2 = animationController->data[names[i].ToString()]->GetData();

			int key = animation->currentkeyFrameIndex;
			//lerp position
			FVector deltaLocation = (*pose2[key + 1]->location - *pose1[key]->location)
				* animation->keyFrames[key]->deltaKeyframe
				+ *pose1[key]->location;

			deltaLocation += h.basePose[i].GetLocation() - deltaLocation;

			//lerp scale
			float deltaScale = (pose2[key + 1]->scale - pose1[key]->scale)
				* animation->keyFrames[animation->currentkeyFrameIndex]->deltaKeyframe
				+ pose1[key]->scale;

			//lerp rotation 
			FVector deltaRot =
				(*pose2[key + 1]->rotaion - *pose1[key]->rotaion)
				* animation->keyFrames[animation->currentkeyFrameIndex]->deltaKeyframe
				+ *pose1[key]->rotaion;

			FRotator DeltaRotator(deltaRot.X, deltaRot.Y, deltaRot.Z);

			FQuat DeltaRotationQuat = DeltaRotator.Quaternion();

			FQuat testquat = DeltaRotationQuat * h.basePose[i].GetRotation();

			FTransform animPose;
			animPose.SetLocation(deltaLocation);
			animPose.SetRotation(testquat);
			animPose.SetScale3D(FVector(deltaScale));

			/*int baseBoneIndex = mesh->SkeletalMesh->RefSkeleton.FindBoneIndex((FName)animBonesNames[i]);
			if (baseBoneIndex != -1)
			{
				FTransform refBoneBaseTransform = mesh->SkeletalMesh->RefSkeleton.GetRefBonePose()[baseBoneIndex];
				animPose.SetLocation(animPose.GetLocation() - refBoneBaseTransform.GetLocation());
				animPose.SetRotation(animPose.GetRotation() * refBoneBaseTransform.GetRotation());
				animPose.SetScale3D(FVector(deltaScale));
			}*/

			FName parentName = h.FindBoneByName(names[i])->GetParentName(); //gets the parent bone from the heriarchy
			//FName parentName = mesh->GetParentBone((FName)animationController->bonesNames->GetData()[i]);
			if (parentName == NAME_None)
			{
				ForwardKinematics::SolveRootFK(mesh, animPose, names[i]);
			}
			else
			{
				ForwardKinematics::SolveSingleFK(mesh, animPose, names[i], parentName);
			}
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

	FTransform parentT = mesh->GetBoneTransformByName(parentName, EBoneSpaces::ComponentSpace);

	mesh->SetBoneTransformByName(name, FTransform(animPose.ToMatrixWithScale() * parentT.ToMatrixWithScale()), EBoneSpaces::ComponentSpace);
	return 1;
}