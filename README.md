# TH-Valley
Final Project of TJU Program Paradigm: A Stardew Valley Style Farm Life Simulation RPG Game

## Agriculture
---
Brief: The basic class supporting agriculture in game.

### Member

- CurrentYear
- CurrentMonth
- CurrentDay
- CurrentHour
- CurrentSeason
- CurrentWeather
- Temperature

## CropProduction
---
Brief: The derived class from Agriculture

### Member

- PlayerCropLevel
- TotalCrops
- Crops(vector)

## Crops
---
Brief: The derived class from Agriculture

### Member

- CurrentGrowthStage
- MaxGrowthStage
- GrowthDuration(vector)
- GrowthSituation
- WaterRequirement
- FertilizerRequirement
- SoilRequirement
- Position
- PlayerPosition

### Member Function

#### showCropInfo
Brief: A virtual function. Show the information of crops, and offer the operation for crops like watering and fertilizing

## Strawberry
---
Brief: An example of the specific crops, the derived class from Crops.

- spritePng
