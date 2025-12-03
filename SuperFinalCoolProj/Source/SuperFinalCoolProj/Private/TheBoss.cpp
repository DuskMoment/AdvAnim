// Fill out your copyright notice in the Description page of Project Settings.

#include "TheBoss.h"
#include "AnimationCont.h"

// Sets default values
ATheBoss::ATheBoss() : AActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("mesh"));
	skelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skelMeshComp"));
	
	SetRootComponent(mesh);

	//skelMeshComp->SetAnimation(animClip);

}

// Called when the game starts or when spawned
void ATheBoss::BeginPlay()
{
	Super::BeginPlay();

	if (mesh == NULL)
	{
		return;
	}

	if (skelMesh == NULL)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT("skell mesh was null")));
	}

	mesh->SetSkeletalMesh(skelMesh, true);
	skelMeshComp->SetSkeletalMesh(skelMesh, true);

	mesh->CopyPoseFromSkeletalComponent(skelMeshComp);


	controller = new ClipController();

	controller->currClip = animClip;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT("Atempting to init")));

	skelMeshComp->SetAnimInstanceClass(animInst);
	skelMeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);


	if (animInst)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("FOUND THE BP")));
	}
	
	if (!skelMeshComp->HasValidAnimationInstance())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("NO INSTANCE")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("HAS INSTANCE")));
	}

	mesh->GetBoneNames(names);
	mesh->bDisplayBones = true;

	size_t boneCount = mesh->GetNumBones();
	for (int i = 0; i < names.Num(); i++)
	{
		//get all of the data and plasice it into a struct
		FTransform trans = mesh->GetBoneTransform(names[i], RTS_Component);
		FVector loc = mesh->GetBoneLocation(names[i], EBoneSpaces::ComponentSpace);
		FQuat quat = mesh->GetBoneQuaternion(names[i], EBoneSpaces::ComponentSpace);
		FVector scale = trans.GetScale3D();

		BoneDataNode node(trans, loc, quat, scale);
		node.SetBoneName(names[i]);

		node.SetParentName(mesh->GetParentBone(node.GetBoneName()));
		data.Add(node);

		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone Data loaded %s %f"), *node.GetBoneName().ToString(), (float)node.GetLocation().X));*/
	
		/*FAnimationPoseData* t;
		animClip->GetPlayLength();
		animClip->GetAnimationPose(*t, FAnimExtractContext());*/


	}

	pH = new Heirarchy(&data);

	if (pH->GetBoneCount() != 0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Heirarchy Populated %s"), *data[0].GetBoneName().ToString()));
	}
	

	controller->GetAssetData(mesh, skelMeshComp);
}

// Called every frame
void ATheBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	controller->UpdateClipController(DeltaTime);

	//RightUpLeg
	BoneDataNode* node = pH->FindBoneByName("RightUpLeg");
	mesh->SetBoneLocationByName("RightUpLeg", node->GetLocation() + (FVector::UpVector * 10), EBoneSpaces::ComponentSpace);

	node->SetLocation(node->GetLocation() + (FVector::UpVector * 10));
	controller->GetAssetData(mesh, skelMeshComp);
}

void ATheBoss::ApplyChangeToMesh(FName boneName)
{
	BoneDataNode* node = pH->FindBoneByName(boneName);

	//applay changes
}

