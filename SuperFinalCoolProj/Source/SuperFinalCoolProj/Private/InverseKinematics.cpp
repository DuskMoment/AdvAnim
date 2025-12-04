// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematics.h"

int InverseKinematics::UpdateFullIK(UModifiedPoseableMeshComponent* mesh, FCompactPose& animPoses)
{
	InverseKinematics::SolvePartialIK(mesh, animPoses);
	return 1;
}

int InverseKinematics::SolvePartialIK(UModifiedPoseableMeshComponent* mesh, FCompactPose& animPoses)
{
	TArray<FName> names;
	mesh->GetBoneNames(names);

	for (int i = 0; i < names.Num(); i++)
	{
		FName parentName = mesh->GetParentBone(names[i]);
		if (parentName == NAME_None)
		{
			InverseKinematics::SolveRootIK(mesh, animPoses, names[i]);
		}
		else
		{
			InverseKinematics::SolveSingleIK(mesh, animPoses, names[i], parentName);
		}
	}

	return 1;
}

int InverseKinematics::SolveRootIK(UModifiedPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name)
{
	mesh->SetBoneSpaceTranformByName(mesh->GetBoneTransformByName(name, EBoneSpaces::ComponentSpace), name);
	return 1;
}

int InverseKinematics::SolveSingleIK(UModifiedPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name, FName parentName)
{
	FTransform parentInvT = mesh->GetBoneTransformByName(parentName, EBoneSpaces::ComponentSpace).Inverse();
	FTransform currentT = mesh->GetBoneTransformByName(name, EBoneSpaces::ComponentSpace);

	mesh->SetBoneSpaceTranformByName(currentT * parentInvT, name);
	return 1;
}

int InverseKinematics::UpdateEffectors(UModifiedPoseableMeshComponent* mesh)
{
	InverseKinematics::UpdateLookAt(mesh);
	return 1;
}

void InverseKinematics::ResolvePostEffectorIK(UModifiedPoseableMeshComponent* mesh, FTransform jToObj, FName name)
{
	mesh->SetBoneRotationByName(name, jToObj.GetRotation().Rotator(), EBoneSpaces::WorldSpace);
}

void InverseKinematics::UpdateLookAt(UModifiedPoseableMeshComponent* mesh)
{
	FName name("Neck");
	FTransform jToObj = mesh->GetBoneTransformByName(name, EBoneSpaces::WorldSpace);

	//Hardcoded effector world position for now
	FVector effectorInWorld = FVector(-600.000000, 6660.000000, 90.000000);

	//Making the look at basis
	FVector fwd = effectorInWorld - mesh->GetBoneTransformByName(name, EBoneSpaces::WorldSpace).GetLocation();
	fwd.Normalize();

	FVector right = FVector::CrossProduct(FVector::UpVector, fwd);
	right.Normalize();

	FVector up = FVector::CrossProduct(right, fwd);
	up.Normalize();

	FMatrix lookAt_Basis;
	lookAt_Basis.SetColumn(0, right);
	lookAt_Basis.SetColumn(1, fwd);
	lookAt_Basis.SetColumn(2, up);

	//jToObj.SetRotation(fwd.Rotation().Quaternion());

	//Get the affected joint basis and get it's transpose (transpose is inverse for orthonormal basis)
	FMatrix basisT = mesh->GetBoneTransformByName(name, EBoneSpaces::WorldSpace).GetRotation().ToMatrix().GetTransposed();

	//Make look at basis relative to bone basis
	jToObj.SetRotation((basisT * lookAt_Basis).ToQuat());

	//Solve post IK 
	InverseKinematics::ResolvePostEffectorIK(mesh, jToObj, name);
}


