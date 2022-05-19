#include "MyJrpgEditor/Public/MyJrpgEditor.h"

#include "DataTableEditorUtils.h"
#include "DataTableRowHandleCustom.h"
#include "MyJrpg/DataTables/SkillData.h"


class OnDataTableChanged :public INotifyOnChanged<UDataTable,FDataTableEditorUtils::EDataTableChangeInfo>//,public FDataTableEditorUtils::INotifyOnDataTableChanged
{
public:
	virtual ~OnDataTableChanged() 
	{
		Module = nullptr;
	}
	
	FMyJrpgEditorModule* Module;
	
	virtual void PreChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo ChangedType)override
	{
		//Module->OnAllPlaceableAssetsChanged();
	}
	virtual void PostChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo ChangedType)override
	{
		// if(!Changed->RowStruct->IsChildOf(FSkillDataRow::StaticStruct()) && !Changed->RowStruct->IsChildOf(FPlayerUnitEntityRow::StaticStruct()))
		// {
		// 	return;//스킬테이블이 바뀌면
		// }
		// Module->RefreshPlayerSkillData();//플레이어 테이블 변경
	}
	
	virtual void SelectionChange(const UDataTable* DataTable, FName RowName)
	{
		
	}
};

void FMyJrpgEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	PropertyModule.RegisterCustomPropertyTypeLayout("MonsterUnitEntityRowHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableRowHandleCustom::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("ItemDataHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableRowHandleCustom::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("DropDataRowHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableRowHandleCustom::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("OptionDataHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableRowHandleCustom::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("ColorDataHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableRowHandleCustom::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("CurrencyDataHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableRowHandleCustom::MakeInstance));


	m_OnDataTableChanged = MakeShared<OnDataTableChanged>();

	m_OnDataTableChanged->Module = this;
	
	FDataTableEditorUtils::FDataTableEditorManager::Get().AddListener(m_OnDataTableChanged.Get());

	RefreshPlayerSkillData();
	
	PropertyModule.NotifyCustomizationModuleChanged();
}//FCurrencyDataHandle

void FMyJrpgEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	PropertyModule.UnregisterCustomPropertyTypeLayout("MonsterUnitEntityRowHandle");
	PropertyModule.UnregisterCustomPropertyTypeLayout("ItemDataHandle");
	PropertyModule.UnregisterCustomPropertyTypeLayout("DropDataRowHandle");
	PropertyModule.UnregisterCustomPropertyTypeLayout("OptionDataHandle");
	PropertyModule.UnregisterCustomPropertyTypeLayout("ColorDataHandle");
	PropertyModule.UnregisterCustomPropertyTypeLayout("CurrencyDataHandle");

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FMyJrpgEditorModule::RefreshPlayerSkillData()
{
	const TArray<FName> ArySkillNames = USkillData::GetSkillTable->GetRowNames();
	
	// for(TObjectIterator<UDataTable> classIt(EObjectFlags::RF_ClassDefaultObject,false,EInternalObjectFlags::None);classIt;++classIt)
	// {
	// 	if(!classIt->RowStruct->IsChildOf(FPlayerUnitEntityRow::StaticStruct()))
	// 	{
	// 		continue;
	// 	}
	// 	TArray<FPlayerUnitEntityRow*> AryUnits;
	// 	
	// 	classIt->GetAllRows(TEXT(""),AryUnits);
	//
	// 	for(FPlayerUnitEntityRow* Pl : AryUnits)
	// 	{
	// 		int Diff = ArySkillNames.Num() - Pl->m_ArySkillAnims.Num();
	//
	// 		while (Diff<0)
	// 		{
	// 			Pl->m_ArySkillAnims.Pop();
	// 			Diff++;
	// 		}
	// 		while (Diff>0)
	// 		{
	// 			Pl->m_ArySkillAnims.Add(FPlayerSkillAnim());
	// 			Diff--;
	// 		}
	// 		for(int i=0; i < ArySkillNames.Num(); i++)
	// 		{
	// 			Pl->m_ArySkillAnims[i].m_SkillID = ArySkillNames[i];
	// 		}
	// 	}
	// }
}

IMPLEMENT_GAME_MODULE(FMyJrpgEditorModule,MyJrpgEditor);