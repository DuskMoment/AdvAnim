// Fill out your copyright notice in the Description page of Project Settings.


#include "BlendTree.h"

BlendTree::BlendTree()
{
}

BlendTree::~BlendTree()
{
}

void BlendTree::CreateTree(UModifiedPoseableMeshComponent* meshHeirarchy)
{
	blendTree.heirarchyDescriptor = meshHeirarchy;
	blendTree.nodes.Init({}, meshHeirarchy->GetNumBones());
}

void BlendTree::ConfigureNode(FName name, FTransform* outPose, FTransform* inPose1, FTransform* inPose2, BlendOps blendOp)
{
	int nodeIndex = blendTree.heirarchyDescriptor->GetBoneIndex(name);

	//Maybe have a way to pick another node struct that uses a different amount of ctrls and inputs for blend op
	BlendNode2C1I newNode;
	newNode.out = outPose;
	newNode.ctrl[0] = inPose1;
	newNode.ctrl[1] = inPose2;

	blendTree.nodes[nodeIndex] = newNode;
}

void BlendTree::Execute(float u)
{
	TArray<FName> poseNames;
	blendTree.heirarchyDescriptor->GetBoneNames(poseNames);
	for (int i = 0; i < poseNames.Num(); i++)
	{
		BlendNode2C1I node = blendTree.nodes[i];
		{
			/*FTransform outPose;
			FTransform pose1 = blendTree.heirarchyDescriptor->GetBoneTransform(node2C1I->ctrl[0]);
			FTransform pose2 = blendTree.heirarchyDescriptor->GetBoneTransform(node2C1I->ctrl[1]);*/
			node.out->SetTranslation(Lerp(node.ctrl[1]->GetTranslation(), node.ctrl[0]->GetTranslation(), u));
			/*FVector blendedEuler = FMath::Lerp(node.ctrl[1]->Rotator().Euler(), node.ctrl[0]->Rotator().Euler(), u);
			FRotator blendedRotator(blendedEuler.X, blendedEuler.Y, blendedEuler.Z);
			node.out->SetRotation(blendedRotator.Quaternion());*/
			node.out->SetRotation(FMath::Lerp(node.ctrl[1]->GetRotation(), node.ctrl[0]->GetRotation(), u));
			node.out->SetScale3D(Lerp(node.ctrl[1]->GetScale3D(), node.ctrl[0]->GetScale3D(), u));


			/*node.out->SetLocation(node.ctrl[0]->GetLocation());
			node.out->SetRotation(node.ctrl[0]->GetRotation());
			node.out->SetScale3D(node.ctrl[0]->GetScale3D());*/
			//blendTree.heirarchyDescriptor->SetBoneSpaceTranformByName(outPose, node2C1I->out);
		}
	}
}

FVector BlendTree::Lerp(FVector v0, FVector v1, float u)
{
	FVector out;
	out.X = (v1.X - v0.X) * u + v0.X;
	out.Y = (v1.Y - v0.Y) * u + v0.Y;
	out.Z = (v1.Z - v0.Z) * u + v0.Z;
	return out;
}

