// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MyFirstActor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeMyFirstActor() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor();
SUPERFINALCOOLPROJ_API UClass* Z_Construct_UClass_AMyFirstActor();
SUPERFINALCOOLPROJ_API UClass* Z_Construct_UClass_AMyFirstActor_NoRegister();
UPackage* Z_Construct_UPackage__Script_SuperFinalCoolProj();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AMyFirstActor ************************************************************
void AMyFirstActor::StaticRegisterNativesAMyFirstActor()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_AMyFirstActor;
UClass* AMyFirstActor::GetPrivateStaticClass()
{
	using TClass = AMyFirstActor;
	if (!Z_Registration_Info_UClass_AMyFirstActor.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("MyFirstActor"),
			Z_Registration_Info_UClass_AMyFirstActor.InnerSingleton,
			StaticRegisterNativesAMyFirstActor,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AMyFirstActor.InnerSingleton;
}
UClass* Z_Construct_UClass_AMyFirstActor_NoRegister()
{
	return AMyFirstActor::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AMyFirstActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "MyFirstActor.h" },
		{ "ModuleRelativePath", "Public/MyFirstActor.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMyFirstActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AMyFirstActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_SuperFinalCoolProj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMyFirstActor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AMyFirstActor_Statics::ClassParams = {
	&AMyFirstActor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009001A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMyFirstActor_Statics::Class_MetaDataParams), Z_Construct_UClass_AMyFirstActor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AMyFirstActor()
{
	if (!Z_Registration_Info_UClass_AMyFirstActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMyFirstActor.OuterSingleton, Z_Construct_UClass_AMyFirstActor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AMyFirstActor.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AMyFirstActor);
AMyFirstActor::~AMyFirstActor() {}
// ********** End Class AMyFirstActor **************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h__Script_SuperFinalCoolProj_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AMyFirstActor, AMyFirstActor::StaticClass, TEXT("AMyFirstActor"), &Z_Registration_Info_UClass_AMyFirstActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMyFirstActor), 149139450U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h__Script_SuperFinalCoolProj_1304518238(TEXT("/Script/SuperFinalCoolProj"),
	Z_CompiledInDeferFile_FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h__Script_SuperFinalCoolProj_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h__Script_SuperFinalCoolProj_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
