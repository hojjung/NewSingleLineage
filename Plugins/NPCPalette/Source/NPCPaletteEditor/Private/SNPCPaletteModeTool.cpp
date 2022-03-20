// My First Hack n Slash


#include "SNPCPaletteModeTool.h"
#include "SlateOptMacros.h"
#include "Application/SlateApplicationBase.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Input/SCheckBox.h"
#include "EditorStyleSet.h"
#include "EditorModeManager.h"
#include "EditorModes.h"
#include "AssetThumbnail.h"
#include "LevelEditor.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "EditorClassUtils.h"
#include "NPCPaletteEditorEdMode.h"
#include "FNPCPlacementInfo.h"
#include "Widgets/Input/SSearchBox.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


namespace NPCPaletteViewFilter
{
	void GetBasicStrings(const FNPCPaletteItem& InPlaceableItem, TArray<FString>& OutBasicStrings)
	{
		OutBasicStrings.Add(InPlaceableItem.DisplayName.ToString());

		const FString* SourceString = FTextInspector::GetSourceString(InPlaceableItem.DisplayName);
		if (SourceString)
		{
			OutBasicStrings.Add(*SourceString);
		}
	}
} // namespace PlacementViewFilter

class SPlacementAssetThumbnail : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlacementAssetThumbnail)
				: _Width(32)
				, _Height(32)
				, _AlwaysUseGenericThumbnail(false)
				, _AssetTypeColorOverride()
	{
	}

	SLATE_ARGUMENT(uint32, Width)

	SLATE_ARGUMENT(uint32, Height)

	SLATE_ARGUMENT(FName, ClassThumbnailBrushOverride)

	SLATE_ARGUMENT(bool, AlwaysUseGenericThumbnail)

	SLATE_ARGUMENT(TOptional<FLinearColor>, AssetTypeColorOverride)
SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedPtr<const FNPCPaletteItem>&  item)
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		TSharedPtr<FAssetThumbnailPool> ThumbnailPool = LevelEditorModule.GetFirstLevelEditor()->GetThumbnailPool();

		Thumbnail = MakeShareable(new FAssetThumbnail(item.Get()->m_SelectedEntityRow->m_Icon.LoadSynchronous(), InArgs._Width, InArgs._Height, ThumbnailPool));

		FAssetThumbnailConfig Config;
		Config.bForceGenericThumbnail = InArgs._AlwaysUseGenericThumbnail;
		Config.ClassThumbnailBrushOverride = InArgs._ClassThumbnailBrushOverride;
		Config.AssetTypeColorOverride = InArgs._AssetTypeColorOverride;
		ChildSlot
		[
			Thumbnail->MakeThumbnailWidget(Config)
		];
	}

private:
	TSharedPtr<FAssetThumbnail> Thumbnail;
};


struct FSortPlaceableItems
{
	static bool SortItemsByOrderThenName(const TSharedPtr<FNPCPaletteItem>& A, const TSharedPtr<FNPCPaletteItem>& B)
	{
		if (A->SortOrder.IsSet())
		{
			if (B->SortOrder.IsSet())
			{
				return A->SortOrder.GetValue() < B->SortOrder.GetValue();
			}
			else
			{
				return true;
			}
		}
		else if (B->SortOrder.IsSet())
		{
			return false;
		}
		else
		{
			return SortItemsByName(A, B);
		}
	}

	static bool SortItemsByName(const TSharedPtr<FNPCPaletteItem>& A, const TSharedPtr<FNPCPaletteItem>& B)
	{
		return A->DisplayName.CompareTo(B->DisplayName) < 0;
	}
};



void SNPCPaletteEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FNPCPaletteItem>& InItem)
{
	bIsPressed = false;

	Item = InItem;


	const FButtonStyle& ButtonStyle = FEditorStyle::GetWidgetStyle<FButtonStyle>("PlacementBrowser.Asset");

	NormalImage = &ButtonStyle.Normal;
	HoverImage = &ButtonStyle.Hovered;
	PressedImage = &ButtonStyle.Pressed;

	

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(this, &SNPCPaletteEntry::GetBorder)
		.Cursor(EMouseCursor::GrabHand)
		//.ToolTip(AssetEntryToolTip)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			  .Padding(0)
			  .AutoWidth()
			[
				// Drop shadow border
				SNew(SBorder)
				.Padding(4)
				.BorderImage(FEditorStyle::GetBrush("ContentBrowser.ThumbnailShadow"))
				[
					SNew(SBox)
					.WidthOverride(35)
					.HeightOverride(35)
					[
						SNew(SPlacementAssetThumbnail, Item)
						//.ClassThumbnailBrushOverride(Item->ClassThumbnailBrushOverride)
						.AlwaysUseGenericThumbnail(Item->bAlwaysUseGenericThumbnail)
						.AssetTypeColorOverride(Item->AssetTypeColorOverride)
					]
				]
			]

			+ SHorizontalBox::Slot()
			  .VAlign(VAlign_Center)
			  .Padding(2, 0, 4, 0)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				  .Padding(0, 0, 0, 1)
				  .AutoHeight()
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Asset.Name")
					.Text(Item->DisplayName)
					.HighlightText(InArgs._HighlightText)
				]
			]

			// + SHorizontalBox::Slot()
			//   .VAlign(VAlign_Center)
			//   .AutoWidth()
			// [
			// 	DocWidget
			// ]
		]
	];
}

FReply SNPCPaletteEntry::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FNPCPaletteEdMode* EdMode = (FNPCPaletteEdMode*)GLevelEditorModeTools().GetActiveMode(
		FNPCPaletteEdMode::EM_NPCPaletteEdModeId);

	if (!EdMode->GetNPCAsset()) //선택된 에셋이 없으면 드래그 안됨
	{
		return FReply::Handled();
	}
	
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsPressed = true;

		return FReply::Handled().DetectDrag(SharedThis(this), MouseEvent.GetEffectingButton());
	}

	return FReply::Unhandled();
}

FReply SNPCPaletteEntry::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsPressed = false;
	}

	return FReply::Unhandled();
}

FReply SNPCPaletteEntry::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bIsPressed = false;

	FNPCPaletteEdMode* EdMode = (FNPCPaletteEdMode*)GLevelEditorModeTools().GetActiveMode(FNPCPaletteEdMode::EM_NPCPaletteEdModeId);

	if(!EdMode->GetNPCAsset())//선택된 에셋이 없으면 드래그 안됨
	{
		return FReply::Handled(); 
	}

	if (FEditorDelegates::OnAssetDragStarted.IsBound())
	{
		EdMode->SelectDropEntity(*Item.Get());
		
		TArray<FAssetData> DraggedAssetDatas;
		DraggedAssetDatas.Add(Item->AssetData);
		FEditorDelegates::OnAssetDragStarted.Broadcast(DraggedAssetDatas, Item->Factory);
		return FReply::Handled();
	}

	return FReply::Handled();
}

bool SNPCPaletteEntry::IsPressed() const
{
	return bIsPressed;
}

const FSlateBrush* SNPCPaletteEntry::GetBorder() const
{
	if (IsPressed())
	{
		return PressedImage;
	}
	else if (IsHovered())
	{
		return HoverImage;
	}
	else
	{
		return NormalImage;
	}
}

SNPCPaletteModeTool::~SNPCPaletteModeTool()
{
	if (INPCPaletteEditorModule::IsAvailable())
	{
		INPCPaletteEditorModule::Get().OnRecentlyPlacedChanged().RemoveAll(this);
		INPCPaletteEditorModule::Get().OnAllPlaceableAssetsChanged().RemoveAll(this);
	}
}

void SNPCPaletteModeTool::Construct(const FArguments& InArgs)
{
	bPlaceablesFullRefreshRequested = false;
	bRecentlyPlacedRefreshRequested = false;
	bNeedsUpdate = true;

	FNPCPaletteEdMode* PlacementEditMode = (FNPCPaletteEdMode*)GLevelEditorModeTools().GetActiveMode(
		FNPCPaletteEdMode::EM_NPCPaletteEdModeId);
	PlacementEditMode->AddValidFocusTargetForPlacement(SharedThis(this));

	SearchTextFilter = MakeShareable(new FPlacementAssetEntryTextFilter(
		FPlacementAssetEntryTextFilter::FItemToStringArray::CreateStatic(&NPCPaletteViewFilter::GetBasicStrings)
	));

	TSharedRef<SVerticalBox> Tabs = SNew(SVerticalBox).Visibility(this, &SNPCPaletteModeTool::GetTabsVisibility);

	// Populate the tabs and body from the defined placeable items
	INPCPaletteEditorModule& PlacementModeModule = INPCPaletteEditorModule::Get();

	TArray<FNPCEntityCategoryInfo> Categories;
	PlacementModeModule.GetSortedCategories(Categories);
	for (const FNPCEntityCategoryInfo& Category : Categories)
	{
		Tabs->AddSlot()
		    .AutoHeight()
		[
			CreatePlacementGroupTab(Category)
		];
	}

	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar)
		.Thickness(FVector2D(9.0f, 9.0f));
	//Slate start
	ChildSlot
	[
		SNew(SVerticalBox)
		
		+ SVerticalBox::Slot()//에셋 드롭다운
		  .Padding(4)
		  .AutoHeight()
		[
			SAssignNew(m_AssetEntry, SObjectPropertyEntryBox)
			.AllowedClass(UNPCPaletteDataAsset::StaticClass())
			.OnObjectChanged(this, &SNPCPaletteModeTool::OnAssetDropped)
			.OnShouldFilterAsset(this, &SNPCPaletteModeTool::ShouldFilterAsset)
			.ObjectPath(this, &SNPCPaletteModeTool::GetObjectPath)
		]
		//
		+ SVerticalBox::Slot()//검색창
		  .Padding(0)
		  .AutoHeight()
		[
			SAssignNew(SearchBoxPtr, SSearchBox)
			.HintText(NSLOCTEXT("PlacementMode", "SearchPlaceables", "Search Classes"))
			.OnTextChanged(this, &SNPCPaletteModeTool::OnSearchChanged)
			.OnTextCommitted(this, &SNPCPaletteModeTool::OnSearchCommitted)
		]

		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SHorizontalBox)//엔티티 배치용 첫번째 수평

			+ SHorizontalBox::Slot()//카테고리 선택 탭
			.AutoWidth()
			[
				Tabs
			]

			+ SHorizontalBox::Slot()//엔티티 목록
			[
				SNew(SBorder)
				.Padding(FMargin(3))
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
				[
					SNew(SOverlay)

					+ SOverlay::Slot()
					  .HAlign(HAlign_Center)
					  .VAlign(VAlign_Fill)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("PlacementMode", "NoResultsFound", "No Results Found"))
						.Visibility(this, &SNPCPaletteModeTool::GetFailedSearchVisibility)
					]

					+ SOverlay::Slot()
					[
						SAssignNew(CustomContent, SBox)
					]

					+ SOverlay::Slot()
					[
						SAssignNew(DataDrivenContent, SBox)
						[
							SNew(SHorizontalBox)

							+ SHorizontalBox::Slot()
							[
								SAssignNew(ListView, SListView<TSharedPtr<FNPCPaletteItem>>)//실제 목록 리스트 뷰
								.ListItemsSource(&FilteredItems)
								.OnGenerateRow(this, &SNPCPaletteModeTool::OnGenerateWidgetForItem)
								.ExternalScrollbar(ScrollBar)
							]

							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								ScrollBar
							]
						]
					]
				]
			]
		]
	];

	//Slate end

	ActiveTabName = FNPCPalettePlacementCategories::AllClasses();
	bNeedsUpdate = true;

	PlacementModeModule.OnRecentlyPlacedChanged().AddSP(this, &SNPCPaletteModeTool::UpdateRecentlyPlacedAssets);
	PlacementModeModule.OnAllPlaceableAssetsChanged().AddSP(this, &SNPCPaletteModeTool::UpdatePlaceableAssets);

	UpdatePlaceableAssets();
}

TSharedRef<SWidget> SNPCPaletteModeTool::CreatePlacementGroupTab(const FNPCEntityCategoryInfo& Info)
{
	return SNew(SCheckBox)
	.Style(FEditorStyle::Get(), "PlacementBrowser.Tab")
	.OnCheckStateChanged(this, &SNPCPaletteModeTool::OnPlacementTabChanged, Info.UniqueHandle)
	.IsChecked(this, &SNPCPaletteModeTool::GetPlacementTabCheckedState, Info.UniqueHandle)
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SSpacer)
			.Size(FVector2D(1, 30))
		]

		+ SOverlay::Slot()
		  .Padding(FMargin(6, 0, 15, 0))
		  .VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
			.Text(Info.DisplayName)
		]

		+ SOverlay::Slot()
		  .VAlign(VAlign_Fill)
		  .HAlign(HAlign_Left)
		[
			SNew(SImage)
			.Image(this, &SNPCPaletteModeTool::PlacementGroupBorderImage, Info.UniqueHandle)
		]
	];
}

FName SNPCPaletteModeTool::GetActiveTab() const
{
	return IsSearchActive() ? FNPCPalettePlacementCategories::AllClasses() : ActiveTabName;
}

void SNPCPaletteModeTool::UpdateFilteredItems()
{
	bNeedsUpdate = false;

	INPCPaletteEditorModule& PlacementModeModule = INPCPaletteEditorModule::Get();

	const FNPCEntityCategoryInfo* Category = PlacementModeModule.GetRegisteredPlacementCategory(GetActiveTab());
	if (!Category)
	{
		return;
	}
	else if (Category->CustomGenerator)
	{
		CustomContent->SetContent(Category->CustomGenerator());

		CustomContent->SetVisibility(EVisibility::Visible);
		DataDrivenContent->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		FilteredItems.Reset();

		const FNPCEntityCategoryInfo* CategoryInfo = PlacementModeModule.GetRegisteredPlacementCategory(GetActiveTab());
		if (!ensure(CategoryInfo))
		{
			return;
		}

		if (IsSearchActive())
		{
			auto Filter = [&](const TSharedPtr<FNPCPaletteItem>& Item)
			{
				return SearchTextFilter->PassesFilter(*Item);
			};
			PlacementModeModule.GetFilteredItemsForCategory(CategoryInfo->UniqueHandle, FilteredItems, Filter);

			if (CategoryInfo->bSortable)
			{
				FilteredItems.Sort(&FSortPlaceableItems::SortItemsByName);
			}
		}
		else
		{
			PlacementModeModule.GetItemsForCategory(CategoryInfo->UniqueHandle, FilteredItems);

			if (CategoryInfo->bSortable)
			{
				FilteredItems.Sort(&FSortPlaceableItems::SortItemsByOrderThenName);
			}
		}

		CustomContent->SetVisibility(EVisibility::Collapsed);
		DataDrivenContent->SetVisibility(EVisibility::Visible);
		ListView->RequestListRefresh();
	}
}

bool SNPCPaletteModeTool::IsSearchActive() const
{
	return !SearchTextFilter->GetRawFilterText().IsEmpty();
}

ECheckBoxState SNPCPaletteModeTool::GetPlacementTabCheckedState(FName CategoryName) const
{
	return ActiveTabName == CategoryName ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

EVisibility SNPCPaletteModeTool::GetFailedSearchVisibility() const
{
	if (!IsSearchActive() || FilteredItems.Num())
	{
		return EVisibility::Collapsed;
	}
	return EVisibility::Visible;
}


EVisibility SNPCPaletteModeTool::GetTabsVisibility() const
{
	return IsSearchActive() ? EVisibility::Collapsed : EVisibility::Visible;
}

TSharedRef<ITableRow> SNPCPaletteModeTool::OnGenerateWidgetForItem(TSharedPtr<FNPCPaletteItem> InItem,
                                                                   const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FNPCPaletteItem>>, OwnerTable)
	[
		SNew(SNPCPaletteEntry, InItem.ToSharedRef())
		.HighlightText(this, &SNPCPaletteModeTool::GetHighlightText)
	];
}

void SNPCPaletteModeTool::OnPlacementTabChanged(ECheckBoxState NewState, FName CategoryName)
{
	if (NewState == ECheckBoxState::Checked)
	{
		ActiveTabName = CategoryName;
		INPCPaletteEditorModule::Get().RegenerateItemsForCategory(ActiveTabName);

		bNeedsUpdate = true;
	}
}

const FSlateBrush* SNPCPaletteModeTool::PlacementGroupBorderImage(FName CategoryName) const
{
	if (ActiveTabName == CategoryName)
	{
		static FName PlacementBrowserActiveTabBarBrush("PlacementBrowser.ActiveTabBar");
		return FEditorStyle::GetBrush(PlacementBrowserActiveTabBarBrush);
	}
	else
	{
		return nullptr;
	}
}

void SNPCPaletteModeTool::UpdateRecentlyPlacedAssets(const TArray<FNPCPlacementInfo>& RecentlyPlaced)
{
	if (GetActiveTab() == FNPCPalettePlacementCategories::RecentlyPlaced())
	{
		bRecentlyPlacedRefreshRequested = true;
	}
}

void SNPCPaletteModeTool::UpdatePlaceableAssets()
{
	//if (GetActiveTab() == FNPCPalettePlacementCategories::AllClasses())
	{
		bPlaceablesFullRefreshRequested = true;
	}
}

void SNPCPaletteModeTool::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (bPlaceablesFullRefreshRequested)
	{
		INPCPaletteEditorModule::Get().RegenerateItemsForCategory(FNPCPalettePlacementCategories::AllClasses());
		bPlaceablesFullRefreshRequested = false;
		bNeedsUpdate = true;
	}

	if (bRecentlyPlacedRefreshRequested)
	{
		INPCPaletteEditorModule::Get().RegenerateItemsForCategory(FNPCPalettePlacementCategories::RecentlyPlaced());
		bRecentlyPlacedRefreshRequested = false;
		bNeedsUpdate = true;
	}

	if (bNeedsUpdate)
	{
		UpdateFilteredItems();
	}
}

FReply SNPCPaletteModeTool::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		FNPCPaletteEdMode* PlacementEditMode = (FNPCPaletteEdMode*)GLevelEditorModeTools().GetActiveMode(
			FNPCPaletteEdMode::EM_NPCPaletteEdModeId);
		PlacementEditMode->StopPlacing();
		Reply = FReply::Handled();
	}

	return Reply;
}

void SNPCPaletteModeTool::OnSearchChanged(const FText& InFilterText)
{
	// If the search text was previously empty we do a full rebuild of our cached widgets
	// for the placeable widgets.
	if (!IsSearchActive())
	{
		bPlaceablesFullRefreshRequested = true;
	}
	else
	{
		bNeedsUpdate = true;
	}

	SearchTextFilter->SetRawFilterText(InFilterText);
	SearchBoxPtr->SetError(SearchTextFilter->GetFilterErrorText());
}

void SNPCPaletteModeTool::OnSearchCommitted(const FText& InFilterText, ETextCommit::Type InCommitType)
{
	OnSearchChanged(InFilterText);
}

FText SNPCPaletteModeTool::GetHighlightText() const
{
	return SearchTextFilter->GetRawFilterText();
}

void SNPCPaletteModeTool::OnAssetDropped(const FAssetData& asset)
{
	FNPCPaletteEdMode* PlacementEditMode = (FNPCPaletteEdMode*)GLevelEditorModeTools().GetActiveMode(
		FNPCPaletteEdMode::EM_NPCPaletteEdModeId);

	PlacementEditMode->SetNPCAsset(Cast<UNPCPaletteDataAsset>(asset.GetAsset()));
}

bool SNPCPaletteModeTool::ShouldFilterAsset(const FAssetData& AssetData)
{
	return false;
}

FString SNPCPaletteModeTool::GetObjectPath() const
{
	FNPCPaletteEdMode* PlacementEditMode = (FNPCPaletteEdMode*)GLevelEditorModeTools().GetActiveMode(
		FNPCPaletteEdMode::EM_NPCPaletteEdModeId);


	return PlacementEditMode->GetNPCAsset() ? PlacementEditMode->GetNPCAsset()->GetPathName() : FString("");
}

void SMyWidget::Construct(const FArguments& InArgs)
{
	
	SNew(SOverlay)
	+ SOverlay::Slot()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Top)
	[

	SNew(STextBlock)
	.ShadowColorAndOpacity(FLinearColor::Black)
	.ColorAndOpacity(FLinearColor::Red)
	.ShadowOffset(FIntPoint(-1, 1))
	.Text(FText::FromString("HHHHHHHHHHH"))
	];

}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
