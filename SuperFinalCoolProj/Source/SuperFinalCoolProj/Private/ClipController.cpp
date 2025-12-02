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
	FCompactPose outPose;
	FBlendedCurve outCurve;
	FStackCustomAttributes OutAttr;

	//UPoseableMeshComponent testComp;

	//ENGINE_API FBoneContainer(const TArrayView<const FBoneIndexType>&InRequiredBoneIndexArray, const UE::Anim::FCurveFilterSettings & InCurveFilterSettings, UObject & InAsset);


	//USkeletalMesh* skel = mesh.GetSkeletalMesh_DEPRECATED();
	//skel->GetRefSkeleton().GetBoneContainer();

	//FReferenceSkeleton refSkel = skel->GetRefSkeleton();
	
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

	auto cont = skelMeshComp->GetAnimInstance()->GetRequiredBones();
	outPose.SetBoneContainer(&skelMeshComp->GetAnimInstance()->GetRequiredBones());

	FAnimationPoseData poseData(outPose, outCurve, OutAttr);

	currClip->GetAnimationPose(poseData, FAnimExtractContext(currClipTime));


	const TArray<FTransform>& boneTrans = reinterpret_cast<const TArray<FTransform>&>(outPose.GetBones());

	const FTransform* boneData = boneTrans.GetData();


	for (int i = 0; i < boneTrans.Num(); i++)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone Data loaded %f"), boneData[i].GetLocation().X));
	}

	return new BoneDataNode();
}
