// Fill out your copyright notice in the Description page of Project Settings.


#include "ClipController.h"
#include "EditorFramework/AssetImportData.h"


ClipController::ClipController()
{
}

ClipController::~ClipController()
{
}

void ClipController::UpdateClipController(float dt)
{
	currClipTime += dt * currClip->RateScale; //Add reverse playback here too

	if (currClipTime > currClip->GetPlayLength()) 
	{
		//Handle looping
		currClipTime = 0;
	}

}

BoneDataNode* ClipController::GetAssetData(UPoseableMeshComponent* mesh, USkeletalMeshComponent* skelMeshComp)
{
	FMemMark Mark(FMemStack::Get());
	//get the track data

	TArray<FName> nameArray;

	IAnimationDataController& ctrl = currClip->GetController();
	const IAnimationDataModel* model = ctrl.GetModel();
	model->GetBoneTrackNames(nameArray);
	FName* names = nameArray.GetData();
	TArray<FBoneAnimationTrack> tracks = model->GetBoneAnimationTracks();
	int test = model->GetBoneTrackIndexByName(names[0]);

	/*for (int i = 0; i < tracks.Num(); i++)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Track Names: %s"), *tracks[i].Name.ToString()));
	}*/

	if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Track NamesSES: %s"), currClip->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex()));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Num Ref Bones %i"), currClip->GetSkeleton()->GetReferenceSkeleton().GetNum()));

	TArray<FAnimNotifyTrack> track = currClip->AnimNotifyTracks;
	FAnimNotifyTrack* dat = track.GetData();

	//FVector3f* positions = boneAnimation.InternalTrackData.PosKeys.GetData();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("clip name %s"), *names->ToString()));


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("KeyNames %i"), model->GetNumberOfKeys()));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("bone track index %i %s"), test, *names[1].ToString()));


	
	FCompactPose outPose;
	FBlendedCurve outCurve;
	FStackCustomAttributes OutAttr;
	
	if (!skelMeshComp)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Skel Mesh Comp does not exist")));
		return nullptr;
	}

	if (!skelMeshComp->GetAnimInstance())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Skel Mesh Comp AnimInstance does not exist")));
		return nullptr;
	}

	//Get bone container for required bones to get animation poses for
	outPose.SetBoneContainer(&skelMeshComp->GetAnimInstance()->GetRequiredBones());

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Num Required Bones %i"), skelMeshComp->GetAnimInstance()->GetRequiredBones().GetNumBones()));

	FAnimationPoseData poseData(outPose, outCurve, OutAttr);

	//Gets animation poses
	currClip->GetAnimationPose(poseData, FAnimExtractContext(currClipTime));

	//Updates poseable mesh bones with animation poses
	FCompactPose OutPose = poseData.GetPose();
	for (FCompactPoseBoneIndex BoneIndex : OutPose.ForEachBoneIndex())
	{
		if (BoneIndex.GetInt() < currClip->GetSkeleton()->GetReferenceSkeleton().GetNum()) {
			const FTransform& BoneTransform = OutPose[BoneIndex];
			const FTransform& BaseBoneTransform = currClip->GetSkeleton()->GetReferenceSkeleton().GetBoneAbsoluteTransform(BoneIndex.GetInt());
			const FName BoneName = currClip->GetSkeleton()->GetReferenceSkeleton().GetBoneName(BoneIndex.GetInt());
			//mesh->SetBoneLocationByName(BoneName, BoneTransform.GetLocation() + BaseBoneTransform.GetLocation(), EBoneSpaces::ComponentSpace);
			//mesh->SetBoneRotationByName(BoneName, (BoneTransform.GetRotation() + BaseBoneTransform.GetRotation()).Rotator(), EBoneSpaces::ComponentSpace);
			mesh->SetBoneTransformByName(BoneName, BoneTransform, EBoneSpaces::ComponentSpace);
			
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone: %s, Transform: %s"), *BoneName.ToString(), *BoneTransform.ToString()));*/
		}
	}

	const TArray<FTransform>& boneTrans = reinterpret_cast<const TArray<FTransform>&>(outPose.GetBones());

	const FTransform* boneData = boneTrans.GetData();


	/*for (int i = 0; i < boneTrans.Num(); i++)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone Data loaded %f"), boneData[i].GetLocation().X));
	}*/

	return new BoneDataNode();
}
