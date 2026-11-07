#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrinketPickup.generated.h"

class UTrinketDataAsset;

/**
 * 饰品掉落物的数据载体
 *
 * C++ 只负责：记住"我装的是哪个饰品"
 * 蓝图负责：碰撞、拾取、表现
 */
UCLASS()
class RUSHBATTLE_API ATrinketPickup : public AActor	
{
	GENERATED_BODY()

public:
	ATrinketPickup();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Pickup")
	TObjectPtr<UTrinketDataAsset> TrinketData;

	// 生成后由 LootSubsystem 调用 
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void Initialize(UTrinketDataAsset* InTrinket);

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnInitialized();
};